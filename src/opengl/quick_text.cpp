#include <gk/opengl/quick_text.hpp>

#include <gk/opengl/quick_text_expander.hpp>
#include <gk/opengl/quick_program.hpp>
#include <gk/opengl/quick_text_bff_wrapper.hpp>

#include <raiigl/uniform_variable.hpp>

#include <erc/file_id.h>
#include <erc/package_id.h>
#include <erc/inventory_package.h>

#include <list>
#include <memory>
#include <stdexcept>

#include <iostream>

namespace erc {
  //namespace generated_package {
  // extern const erc::package pack_8bf913d56d3df8ed6940d0c425351897b0facf6c92e9c5d4d324d86eee4c9973;
  // }
  //   const extern erc::package pack_8bf913d56d3df8ed6940d0c425351897b0facf6c92e9c5d4d324d86eee4c9973;
  //
  // const erc::package yop("tip", 0, nullptr);
  //
  //   static const struct initializer
  //   {
  //     initializer() {
  //       std::cout << "[quick_text] pushing pack" << std::endl;
  //       //erc::inventory_package::push( erc::generated_package::pack_8bf913d56d3df8ed6940d0c425351897b0facf6c92e9c5d4d324d86eee4c9973 );
  //
  //       erc::inventory_package::push( yop );
  //     }
  //   } _mypack;
  // }
}

//const erc::package & opengl_pack(erc::generated_package::pack_8bf913d56d3df8ed6940d0c425351897b0facf6c92e9c5d4d324d86eee4c9973);

int qInitResources_shaders();

namespace gk {
  namespace opengl {

    static const erc::package_id package_id( "opengl" );

    // ---- ---- ---- ----

    struct font_ressource
    {
      quick_text_fonts id;
      std::string base_name;
    };

    const static font_ressource font_ressource_list[]
    {
      {quick_text_fonts::CalibriLight_512,  "CalibriLight-32x32-512-lum" },
      {quick_text_fonts::CalibriLight_1024, "CalibriLight-64x64-1024-lum" },
    };
    const static uint font_ressource_list_count( sizeof( font_ressource_list ) / sizeof( font_ressource ) );

    // ---- ---- ---- ----

    struct font_allocated_assoc
    {
      int count = 0;
      quick_text_fonts id;
      quick_text_bbf_wrapper bbf_font;
      inline font_allocated_assoc( quick_text_fonts _id, const std::string & bbf_data_font ) :
        id( _id ), bbf_font( bbf_data_font ) {}
    };

    static std::list<font_allocated_assoc> loaded_fonts;

    inline font_allocated_assoc & create_font( quick_text_fonts id )
    {
      for ( font_allocated_assoc & faa : loaded_fonts )
        if ( faa.id == id )
          return faa;

      for ( uint i( 0 ); i < font_ressource_list_count; ++i )
      {
        //
        if ( font_ressource_list[i].id != id ) continue;
        const font_ressource fr( font_ressource_list[i] );

        //
        const erc::embedded_file * const ef(
          erc::inventory_package::get().get_first_embedded_file(
            package_id.from( "quick_text/" + fr.base_name + ".bff" )
          )
        );

        // std::string file_ressource_path( ":/graphic-toolkit/opengl/quick_text/" + std::string::fromStdString( fr.base_name ) + ".bff" );
        // QFileInfo file_info( file_ressource_path );
        // const bool exist_file { file_info.exists() && file_info.isFile() };

        //
        if ( !ef )
          throw std::runtime_error( "[gk::quick_text] Impossible to locate from ressource the bff texture : " + fr.base_name );

        // //
        // QFile bff_file( file_ressource_path );
        // if ( !bff_file.open( QIODevice::OpenModeFlag::ReadOnly ) )
        //   throw std::runtime_error( "[gk::quick_text] Can't open ressource in ReadOnly : " + file_ressource_path.toStdString() );

        //
        font_allocated_assoc & faa( *loaded_fonts.emplace( loaded_fonts.end(), id, ef->get_proper_data() ) );
        ef->unallocate_proper_data();
        return faa;
      }

      //
      throw std::runtime_error( "[quick_text::create_font] Can't find this font_id : " + std::to_string( uint( id ) ) );
    }

    inline void auto_free_font()
    {
      static const auto remove_lesser_count(
        []( const font_allocated_assoc & faa ) -> bool
      { return faa.count <= 0; }
      );
      loaded_fonts.remove_if( remove_lesser_count );
    }


    // ---- ---- ---- ----

    static const erc::file_id quick_text_shader_vert_erc_id( package_id.from( "quick_text/text_triangles.vert" ) );
    static const erc::file_id quick_text_shader_frag_erc_id( package_id.from( "quick_text/text_triangles.frag" ) );

    struct internal_quick_text_program
    {
     private:
      static std::unique_ptr<internal_quick_text_program> semaphore_up;
     public:
      static inline void init() { if ( !semaphore_up ) semaphore_up = std::unique_ptr<internal_quick_text_program>( new internal_quick_text_program() ); }
      static inline internal_quick_text_program & get() { init(); return *semaphore_up; }

     public:
      raiigl::program text_program;

      raiigl::uniform_variable uniform_vp_matrix;
      raiigl::uniform_variable uniform_text_sampler;
      raiigl::uniform_variable uniform_text_normalisation_scale;

     private:
      internal_quick_text_program();

     public:
      void draw( raiigl::gl330 & gl, const glm::mat4x4 & projection_view );
    };

    std::unique_ptr<internal_quick_text_program> internal_quick_text_program::semaphore_up;

    internal_quick_text_program::internal_quick_text_program() :
      text_program(
        quick_program::open_from_local_erc(
          quick_text_shader_vert_erc_id,
          quick_text_shader_frag_erc_id
        ) ),
      uniform_vp_matrix( text_program, "vp_matrix" ),
      uniform_text_sampler( text_program, "text_sampler" ),
      uniform_text_normalisation_scale( text_program, "text_normalisation_scale" )
    { }

    // ---- ---- ---- ----

    quick_text::quick_text( quick_text_fonts _font_id, bool init_at_construct ) :
      font_id( _font_id ),
      text_heap(
        attrib_pointer( 0, 2, raiigl::data_type::Float, true ), // vertex
        attrib_pointer( 1, 2, raiigl::data_type::Float, true )  // uv
      )
    {
      if ( init_at_construct )
        init_gl();
    }

    quick_text::~quick_text()
    {
      if ( faa_p )
      {
        --faa_p->count;
        faa_p = nullptr;
        bff_font_p = nullptr;
        auto_free_font();
      }

    }

    // ---- ---- ---- ----

    void quick_text::init_gl()
    {
      internal_quick_text_program::init();
      if ( !faa_p )
      {
        faa_p = &create_font( font_id );
        ++faa_p->count;
        bff_font_p = &faa_p->bbf_font;
      }
    }

    // ---- ---- --- ----

    inline void quick_text::check_not_busy() const
    {
      if ( busy )
        throw std::runtime_error( "primitives_heap is currently busy" );
    }

    // ---- ---- --- ----

    void quick_text::draw( raiigl::gl330 & gl, const glm::mat4x4 & projection_view )
    {
      //
      if ( !bff_font_p )
        throw std::runtime_error( "[quick_text::draw] need a valid bbf_wrapper" );
      //if ( !text_program_up )
      //  throw std::runtime_error( "[quick_text::draw] need a valid text_program_up" );

      //
      internal_quick_text_program & iqtp( internal_quick_text_program::get() );

      //
      if ( !text_heap.is_init() )
        text_heap.init_buffer();

      //
      iqtp.text_program.use();
      iqtp.uniform_vp_matrix.set( projection_view );

      //
      gl.activate_texture( raiigl::textures_num::Texture00 );
      bff_font_p->texture.bind();
      //uniform_text_sampler->set<int>( bff_font_p->texture.id - GL_TEXTURE0 );
      iqtp.uniform_text_sampler.set( raiigl::textures_num::Texture00 );
      iqtp.uniform_text_normalisation_scale.set( bff_font_p->font.normal_scale );

      //
      gl.enable( raiigl::gl_capabilities::Blend );
      gl.blend_func( raiigl::blend_func_type::SrcAlpha, raiigl::blend_func_type::OneMinusSrcColor );
      //gl.blend_func( raiigl::blend_func_type::SrcAlpha, raiigl::blend_func_type::OneMinusSrcAlpha );

      //
      text_heap.draw( gl, iqtp.text_program );

      //
      gl.disable( raiigl::gl_capabilities::Blend );

      //
      bff_font_p->texture.unbind();
      iqtp.text_program.unuse();
    }

    // ---- ----

    bool quick_text::empty() const
    {
      return text_heap.empty();
    }

    // ---- ---- ---- ----

    void quick_text::reset()
    {
      return text_heap.reset_all();
    }

    // ---- ---- ---- ----

    void quick_text::lock()
    {
      check_not_busy();
      busy = true;
    }

    void quick_text::unlock()
    {
      busy = false;
    }

    // ---- ---- ---- ----

    text_expander quick_text::complete_text()
    {
      return text_expander( *this );
    }

    text_expander quick_text::complete_text( std::string t )
    {
      return text_expander( *this, std::move( t ) );
    }



  }
}
