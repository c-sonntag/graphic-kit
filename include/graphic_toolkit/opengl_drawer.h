#pragma once

#include <graphic_toolkit/types.h>

#include <vector>
#include <list>
#include <unordered_map>
#include <functional>
#include <memory>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <tuple>
#include <typeinfo>

#include <qglobal.h>
#include <QString>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShaderProgram>


namespace graphic_toolkit {

  std::unique_ptr<QOpenGLShaderProgram> opengl_quick_program( const QString & vertex_path, const QString & fragment_path );

  // ---- ---- ---- ----

  template<typename  ... TListTypes>
  struct opengl_buffer_drawer
  {
   public:
    static constexpr uint nb_types { sizeof...( TListTypes ) };

   public:
    struct draw_range;

   public:
    using BufferDrawer = opengl_buffer_drawer<TListTypes...>;
    using Row = std::tuple<TListTypes...>;
    //using Row = typename type_wrapper<TListTypes...>::first;
    using Rows = std::vector<Row>;
    using RowsRange = std::vector<draw_range>;

   public:
    struct attrib_pointer
    {
     public:
      GLuint gl_location;
      GLint gl_tuple_size;
      GLenum gl_type;
      GLboolean gl_normalized;

     public:
      attrib_pointer(
        GLuint _location,
        GLint _tuple_size,
        GLenum _type,
        GLboolean _normalized
      ) :
        gl_location( std::move( _location ) ),
        gl_tuple_size( std::move( _tuple_size ) ),
        gl_type( std::move( _type ) ),
        gl_normalized( std::move( _normalized ) )
      {}
    };

   protected:
    struct attrib_pointer_strided : public attrib_pointer
    {
     public:
      const std::type_info & ctype;
      GLsizei bytesize;
      size_t offset;
      size_t opposed_offset;

     public:
      template<typename T>
      inline attrib_pointer_strided( attrib_pointer va, size_t _offset, T * ) :
        attrib_pointer( std::move( va ) ),
        ctype( typeid( T ) ),
        bytesize( sizeof( T ) ),
        offset( std::move( _offset ) ),
        opposed_offset( row_size - offset - bytesize ) {
        if ( ( opposed_offset > row_size ) || ( offset > row_size ) )
          throw std::runtime_error( "offset or opposed_offset can be highter than row_size" );
      }
    };

   public:
    struct draw_range
    {
     public:

      struct uniform_set
      {
       protected:
        const std::string name;
        const std::function<void( QOpenGLShaderProgram & )> program_setter;

       public:
        template< class... Args >
        inline uniform_set( std::string _name, Args && ...  args ) :
          name( std::move( _name ) ),
          program_setter( [ = ]( QOpenGLShaderProgram & program ) { program.setUniformValue( name.c_str(), args... ); } )
        {}

       public:
        void inline set( QOpenGLShaderProgram & program ) const {
          program_setter( program );
        }
      };

     public:
      using uniform_sets_t = std::list<uniform_set>;
      using uniform_sets_up_t = std::unique_ptr<uniform_sets_t>;

     protected:

      //struct uniform_set_conditonal
      //{
      // protected:
      //  bool actived;
      //  uniform_sets_t uniform_sets;
      //
      // public:
      //  void inline set( QOpenGLShaderProgram & program ) const {
      //    if ( actived )
      //      for ( const draw_range::uniform_set & us : uniform_sets )
      //        us.set( program );
      //  }
      //};

     public:
      using uniform_sets_conditional_t = std::unordered_map<std::string, uniform_sets_t>;
      using uniform_sets_conditional_up_t = std::unique_ptr<uniform_sets_conditional_t>;
      using uniform_sets_conditional_activation_t = std::unordered_map<std::string, bool>;

     public:
      uint start, count;

     protected:
      uniform_sets_up_t uniform_sets_up;
      uniform_sets_conditional_up_t uniform_sets_conditional_up;

     public:
      inline draw_range( uint _start, uint _count ) :
        start( std::move( _start ) ), count( std::move( _count ) )
      {}

      inline draw_range(
        uint _start, uint _count,
        uniform_sets_up_t _uniform_sets_up, uniform_sets_conditional_up_t _uniform_sets_conditional_up
      ) :
        start( std::move( _start ) ), count( std::move( _count ) ),
        uniform_sets_up( std::move( _uniform_sets_up ) ),
        uniform_sets_conditional_up( std::move( _uniform_sets_conditional_up ) )
      {}

      inline void apply_uniform_sets( const BufferDrawer & bd, QOpenGLShaderProgram & program ) const {
        if ( uniform_sets_up )
          for ( const draw_range::uniform_set & us : *uniform_sets_up )
            us.set( program );
        if ( uniform_sets_conditional_up )
          for ( const typename draw_range::uniform_sets_conditional_t::value_type & usc_pair : *uniform_sets_conditional_up )
            if ( bd.is_condition_uniform_set( usc_pair.first ) )
              for ( const draw_range::uniform_set & usc_us : usc_pair.second )
                usc_us.set( program );
      }
    };

   public:
    static constexpr size_t row_size = sizeof( Row );

   protected:
    std::vector<attrib_pointer_strided> attrib_pointers_strides;

   protected:

    template<typename TAttribPointerTypeLast>
    void insert( size_t offset, attrib_pointer & va ) {
      attrib_pointers_strides.emplace_back( va, offset, static_cast<TAttribPointerTypeLast *>( nullptr ) );
    }

    template<typename TAttribPointerTypeCurrent, typename ...TAttribPointerTypeNext, typename... Args>
    void insert( size_t offset, attrib_pointer & va, Args & ... va_next ) {

      //
      attrib_pointers_strides.emplace_back( va, offset, static_cast<TAttribPointerTypeCurrent *>( nullptr ) );

      //
      insert<TAttribPointerTypeNext...>(
        offset + sizeof( TAttribPointerTypeCurrent ),
        va_next...
      );
    }

   public:
    template< typename... Args >
    inline opengl_buffer_drawer( Args ... attributes ) {

      //
      attrib_pointers_strides.reserve( nb_types );
      insert<TListTypes...>( 0, attributes... );

    }

    ~opengl_buffer_drawer() {
      destroyBuffer();
    }

   private:
    bool writing = false;
    bool initialized = false;
   protected:
    void lockWriting() {
      if ( initialized )
        throw std::runtime_error( "opengl_buffer_drawer is alredy init, can't be update now" );
      writing = true;
    }
    void unlockWriting() {
      writing = false;
    }

   protected:
    struct make_rows_data
    {
     public:
      const GLenum draw_mode;
      Rows rows;
      RowsRange range;
      bool locked;

     public:
      make_rows_data( GLenum _draw_mode ) :
        draw_mode( std::move( _draw_mode ) ),
        locked( false )
      {}
    };

   protected:
    std::list<make_rows_data> to_draw;
    typename draw_range::uniform_sets_conditional_activation_t uniform_sets_conditional_activation;

   protected:
    make_rows_data & get_or_create_mvd( GLenum draw_mode ) {
      for ( make_rows_data & mvd : to_draw ) {
        if ( mvd.draw_mode == draw_mode )
          return mvd;
      }
      make_rows_data & mvd( *to_draw.emplace( to_draw.end(), draw_mode ) );
      return mvd;
    }

   protected:
    struct draw_buffer
    {
      const GLenum draw_mode;
      const RowsRange & range;
      const uint buffer_length, buffer_bytesize;
      QOpenGLBuffer buffer;

      inline draw_buffer( const make_rows_data & mvd ) :
        draw_mode( mvd.draw_mode ), range( mvd.range ),
        buffer_length( mvd.rows.size() ), buffer_bytesize( buffer_length * sizeof( Row ) ) {
        buffer.create();
        buffer.bind();
        buffer.allocate( mvd.rows.data(), static_cast<int>( buffer_bytesize ) );
      }

      ~draw_buffer() {
        buffer.destroy();
      }
    };

   protected:
    std::list<draw_buffer> buffers;

   public:

    void init() {

      //
      if ( initialized )
        return;

      //
      if ( writing )
        throw std::runtime_error( "opengl_buffer_drawer is currently in writing mode" );

      //
      for ( const make_rows_data & mvd : to_draw ) {
        if ( !mvd.rows.empty() ) {
          buffers.emplace_back( mvd );

          // const draw_buffer & db = buffers.back();
          // std::cout << "Add buffer : " << std::endl
          //           << " buffer_length : " << db.buffer_length << std::endl
          //           << " row_size : " << row_size << std::endl
          //           << " buffer_bytesize : " << db.buffer_bytesize << std::endl;
          // std::cout << " attrib Pointer " << " :" << std::endl;
          // for ( const attrib_pointer_strided & aps : attrib_pointers_strides ) {
          //   std::cout << "  gl_location : " << aps.gl_location << std::endl
          //             << "  gl_tuple_size : " << aps.gl_tuple_size << std::endl
          //             << "  gl_type : " << aps.gl_type << std::endl
          //             << "  gl_normalized : " << ( aps.gl_normalized ? "1" : "0" ) << std::endl
          //             << "  offset : " << aps.offset << std::endl
          //             << "  opposed_offset : " << aps.opposed_offset << std::endl
          //             << "  ctype : " << aps.ctype.name() << std::endl
          //             << "  bytesize : " << aps.bytesize << std::endl
          //             << "  --- " << std::endl;
          // }
          // std::cout << std::endl;

        }
      }

      //
      initialized = true;
    }

    inline bool is_init() const {
      return initialized;
    }


   public:

    void reset() {

      //
      destroyBuffer();
      to_draw.clear();

      //
      writing = false;
    }


    void destroyBuffer() {
      if ( !initialized )
        return;

      //
      buffers.clear(); // RAII destroy
      initialized = false;
    }


   public:

    void draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) {

      //
      if ( !initialized )
        return; //init();

      //
      for ( draw_buffer & db : buffers ) {

        //
        db.buffer.bind();

        //
        /**< @see #way3 https://stackoverflow.com/a/39684775  */
        for ( const attrib_pointer_strided & aps : attrib_pointers_strides )
          gl.glVertexAttribPointer(
            aps.gl_location,
            aps.gl_tuple_size,
            aps.gl_type,
            aps.gl_normalized,
            row_size,
            reinterpret_cast<const GLvoid *>( aps.opposed_offset ) /**< std::tuple have data inverted order */
          );

        //
        if ( db.range.empty() )
          gl.glDrawArrays( db.draw_mode, 0, static_cast<int>( db.buffer_length ) );
        else {
          for ( const draw_range & dr : db.range ) {
            dr.apply_uniform_sets( *this, program );
            gl.glDrawArrays( db.draw_mode, static_cast<int>( dr.start ), static_cast<int>( dr.count ) );
          }
        }
      }
    }

   public:
    inline void vertexAttribArray_enableAll( QOpenGLFunctions_3_3_Core & gl ) {
      for ( const attrib_pointer_strided & aps : attrib_pointers_strides )
        gl.glEnableVertexAttribArray( aps.gl_location );
    }
    inline void vertexAttribArray_disableAll( QOpenGLFunctions_3_3_Core & gl ) {
      for ( const attrib_pointer_strided & aps : attrib_pointers_strides )
        gl.glDisableVertexAttribArray( aps.gl_location );
    }

   public:
    inline void auto_draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) {
      vertexAttribArray_enableAll( gl );
      draw( gl, program );
      vertexAttribArray_disableAll( gl );
    }

   public:
    inline void enable_condition_uniform_set( const std::string & condition_name ) {
      uniform_sets_conditional_activation[condition_name] = true;
    }
    inline void disable_condition_uniform_set( const std::string & condition_name ) {
      uniform_sets_conditional_activation[condition_name] = true;
    }
    inline bool is_condition_uniform_set( const std::string & condition_name ) const {
      const auto usca_pair_it = uniform_sets_conditional_activation.find( condition_name );
      return ( usca_pair_it != uniform_sets_conditional_activation.end() ) ?
             usca_pair_it->second : false;
    }

   public:
    struct maker
    {
     protected:
      BufferDrawer & bd;
      make_rows_data & mvd;
      //make_rows_data & mvd;
      const bool need_range;

     protected:
      uint start;
      uint count;
      bool released;
      typename draw_range::uniform_sets_up_t uniform_sets_up;
      typename draw_range::uniform_sets_conditional_up_t uniform_sets_conditional_up;

     public:
      inline maker( BufferDrawer & _bd, make_rows_data & _mvd, bool _need_range ) :
        bd( _bd ),
        mvd( _mvd ),
        need_range( _need_range ),
        start( mvd.rows.size() ),
        count( 0 ), released( false ) {

        //
        if ( mvd.locked )
          throw std::runtime_error( "maker type(" + std::to_string( mvd.draw_mode ) + ") is already locked" );

        //
        mvd.locked = true;
        bd.lockWriting();

      }

     public:
      // Enable move.
      maker( maker && ) = default;

      // Disable copy from lvalue.
      maker( const maker & ) = delete;
      maker & operator=( const maker & ) = delete;

     public:
      ~maker() {
        release();
      }

     protected:
      inline void proceed_add() {
        if ( released )
          throw std::runtime_error( "shape_strip_maker can't write on released maker" );
        ++count;
      }

     public:
      inline void release() {
        if ( released ) return;
        //
        bd.unlockWriting();
        mvd.locked = false;
        released = true;
        //
        if ( !need_range ) {

          //
          const bool have_already_range = !mvd.range.empty();
          const bool have_current_uniform_set = uniform_sets_up || uniform_sets_conditional_up;

          //
          if ( !have_already_range && !have_current_uniform_set )
            return;

          //
          if ( !have_already_range && have_current_uniform_set ) {
            if ( mvd.rows.size() < count )
              throw std::runtime_error( "Waw, total vertice can't be less of count vertice" );
            mvd.range.emplace_back( 0, mvd.rows.size() - count );
          }
        }
        //
        mvd.range.emplace_back( start, count, std::move( uniform_sets_up ), std::move( uniform_sets_conditional_up ) );
      }

     public:
      inline void reserve( size_t n ) {
        mvd.rows.reserve( n );
      }


     public:
      template< class... Args >
      inline void add( Args && ... args ) {
        proceed_add();
        mvd.rows.emplace_back( args... );
      }

      template< class... Args >
      inline void set_uniform( const std::string & var_name, Args... values ) {
        if ( !uniform_sets_up ) {
          typename draw_range::uniform_sets_t * us_p = new typename draw_range::uniform_sets_t();
          uniform_sets_up = typename draw_range::uniform_sets_up_t( us_p );
        }
        uniform_sets_up->emplace_back( var_name, values... );
      }

      template< class... Args >
      //inline void set_uniform_on_condition( const std::string & var_name, Args... values, const std::string & condition_name, bool initial_cond = false ) {
      inline void set_uniform_on_condition( const std::string & condition_name, bool initial_cond, const std::string & var_name, Args... values ) {
        if ( !uniform_sets_conditional_up ) {
          typename draw_range::uniform_sets_conditional_t * usc_p = new typename draw_range::uniform_sets_conditional_t();
          uniform_sets_conditional_up = typename draw_range::uniform_sets_conditional_up_t( usc_p );
        }

        //
        const auto usca_pair_it = bd.uniform_sets_conditional_activation.find( condition_name );
        if ( usca_pair_it == bd.uniform_sets_conditional_activation.end() )
          bd.uniform_sets_conditional_activation.emplace( condition_name, initial_cond );

        //
        auto usc_pair_it = uniform_sets_conditional_up->find( condition_name );
        if ( usc_pair_it == uniform_sets_conditional_up->end() ) {
          auto usc_emplacepair = uniform_sets_conditional_up->emplace( condition_name, typename draw_range::uniform_sets_t() );
          usc_pair_it = usc_emplacepair.first;
        }

        //
        usc_pair_it->second.emplace_back( var_name, values... );
      }

    };

   protected:

#define ADD_METHOD_MAKER( need_range, name, draw_type ) \
  inline maker new_ ## name() { \
    return maker( *this, get_or_create_mvd(draw_type), need_range ); \
  }

   public:

    //
    ADD_METHOD_MAKER( false, points, GL_POINTS )
    ADD_METHOD_MAKER( false, lines, GL_LINES )
    ADD_METHOD_MAKER( false, triangles, GL_TRIANGLES )

    //
    ADD_METHOD_MAKER( true, triangle_strip, GL_TRIANGLE_STRIP )
    ADD_METHOD_MAKER( true, line_strip, GL_LINE_STRIP )
    ADD_METHOD_MAKER( true, line_loop, GL_LINE_LOOP )
    ADD_METHOD_MAKER( true, triangle_fan, GL_TRIANGLE_FAN )

  };
}
