#include <graphic_toolkit/opengl/quick_text.h>

#include <graphic_toolkit/opengl/quick_text_expander.h>
#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/quick_text_bff_wrapper.h>

#include <QFile>
#include <QFileInfo>
#include <QDataStream>

#include <list>
#include <memory>
#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    struct font_ressource
    {
      quick_text_fonts id;
      std::string base_name;
    };

    const static font_ressource font_ressource_list[]
    {
      {quick_text_fonts::CalibriLight_512, "CalibriLight-32x32-512-lum"},
      {quick_text_fonts::Calibri_512_rgb, "CalibriLight-32x32-512-rgb"},
      {quick_text_fonts::CalibriLight_1024, "CalibriLight-64x64-1024-lum"},
    };
    const static uint font_ressource_list_count( sizeof( font_ressource_list ) / sizeof( font_ressource ) );

    // ---- ---- ---- ----

    static quick_program text_program( ":/graphic-toolkit/opengl/quick_text/text_triangles.vert", ":/graphic-toolkit/opengl/quick_text/text_triangles.frag" );

    // ---- ---- ---- ----

    struct font_allocated_assoc
    {
      int count = 0;
      quick_text_fonts id;
      quick_text_bbf_wrapper bbf_font;
      inline font_allocated_assoc( quick_text_fonts _id, QFile & in ) :
        id( _id ), bbf_font( in ) {}
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
        QString file_ressource_path( ":/graphic-toolkit/opengl/quick_text/" + QString::fromStdString( fr.base_name ) + ".bff" );
        QFileInfo file_info( file_ressource_path );
        const bool exist_file { file_info.exists() && file_info.isFile() };

        //
        if ( !exist_file )
          throw std::runtime_error( "[graphic_toolkit::quick_text] Imposible to locate from ressource the bff texture : " + fr.base_name );

        //
        QFile bff_file( file_ressource_path );
        if ( !bff_file.open( QIODevice::OpenModeFlag::ReadOnly ) )
          throw std::runtime_error( "[graphic_toolkit::quick_text] Can't open ressource in ReadOnly : " + file_ressource_path.toStdString() );

        //
        return *loaded_fonts.emplace( loaded_fonts.end(), id, bff_file );
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

    quick_text::quick_text( quick_text_fonts _font_id ) :
      font_id( _font_id ),
      text_heap(
        attrib_pointer( 0, 2, GL_FLOAT, true ), // vertex
        attrib_pointer( 1, 2, GL_FLOAT, true )  // uv
      )
    {}

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

    // ---- ----

    void quick_text::init_gl()
    {
      //
      text_program.build();

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

    void quick_text::draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view )
    {
      //
      if ( !bff_font_p )
        throw std::runtime_error( "[quick_text::draw] need a valid text_texture_up" );

      //
      if ( !text_heap.is_init() )
        text_heap.init_buffer();

      //
      text_program.program.bind();
      text_program.program.setUniformValue( "vp_matrix", projection_view );

      //text_program.program.setUniformValue( "text_position", projection_view );
      //text_program.program.setUniformValue( "text_angle", projection_view );

      //
      bff_font_p->texture.bind();
      text_program.program.setUniformValue( "text_sampler", bff_font_p->texture.textureId() - GL_TEXTURE0 );
      text_program.program.setUniformValue( "text_normalisation_scale", bff_font_p->font.normal_scale );

      //
      gl.glEnable( GL_BLEND );
      gl.glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      //
      text_heap.auto_draw( gl, text_program.program );

      //
      gl.glDisable( GL_BLEND );

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
