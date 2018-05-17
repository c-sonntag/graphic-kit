#include <graphic_toolkit/opengl/quick_text.h>

#include <graphic_toolkit/opengl/quick_program.h>

#include <QFileInfo>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {

    const quick_text::font_property quick_text::fonts_list[]
    {
      {"Times_New_Roman", 512, 512, 32, 32, 32, 255, 1.0f, 1.4f, 1.1f, false },
      {"Calibri", 512, 512, 32, 36, 32, 255, 1.7f, 2.2f, 1.1f, false },
    };

    const uint quick_text::fonts_list_count( sizeof( quick_text::fonts_list ) / sizeof( quick_text::font_property ) );

    // ---- ---- ---- ----

    inline quick_text::font_property quick_text::get_font( uint font_id )
    {
      if ( font_id >= fonts_list_count )
        throw std::runtime_error( "[graphic_toolkit::quick_text] Can't find this font_id" );

      //
      return fonts_list[font_id];
    }


    inline QImage quick_text::get_text_texture( const font_property & font )
    {

      //
      QString texture_bmp_file_path(
        ":/graphic-toolkit/opengl/quick_text/"
        + QString::number( font.width ) + "x" + QString::number( font.height )
        + "-" + QString::fromStdString( font.font_basename )
        + "-" + QString::number( font.cell_width ) + "x" + QString::number( font.cell_height )
        + ".bmp"
      );

      //
      QFileInfo file_info( texture_bmp_file_path );
      const bool exist_file { file_info.exists() && file_info.isFile() };

      //
      if ( !exist_file )
        throw std::runtime_error( "[graphic_toolkit::quick_text] Imposible to locate text texture : " + texture_bmp_file_path.toStdString() );

      //
      return QImage( texture_bmp_file_path );
    }

    // ---- ----

    quick_text::quick_text( quick_text_font_name font_id ) :
      font( get_font( uint( font_id ) ) ),
      text_program_up( quick_program( ":/graphic-toolkit/opengl/quick_text/text_triangles.vert", ":/graphic-toolkit/opengl/quick_text/text_triangles.frag" ) ),
      text_texture( get_text_texture( font ) ),
      text_heap(
        attrib_pointer( 0, 2, GL_FLOAT, true ), // vertex
        attrib_pointer( 1, 2, GL_FLOAT, true )  // uv
      )
    {
      //
      //text_texture.setMinMagFilters(
      //  QOpenGLTexture::Filter::LinearMipMapLinear,
      //  QOpenGLTexture::Filter::Linear
      //);

      text_texture.setMinMagFilters(
        QOpenGLTexture::Filter::Nearest,
        QOpenGLTexture::Filter::Nearest
      );

      text_texture.setWrapMode( QOpenGLTexture::Repeat );

    }

    // ---- ----

    void quick_text::draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view )
    {
      if ( !text_heap.is_init() )
        text_heap.init_buffer();

      //
      text_program_up->bind();
      text_program_up->setUniformValue( "vp_matrix", projection_view );

      //text_program_up->setUniformValue( "text_position", projection_view );
      //text_program_up->setUniformValue( "text_angle", projection_view );

      //
      text_texture.bind();
      text_program_up->setUniformValue( "text_sampler", text_texture.textureId() - GL_TEXTURE0 );

      //
      gl.glEnable( GL_BLEND );
      gl.glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      //
      text_heap.auto_draw( gl, *text_program_up );

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

    void quick_text::add( const std::string & t, const float size, const QVector3D & pos, const QVector3D & degree_angle_3d, const QVector3D & color, quick_text_horizontal_align align_h )
    {
      const uint t_length( t.size() );

      //
      const uint nb_carac_per_column( font.height / font.cell_height );
      const uint nb_carac_per_line( font.width / font.cell_width );

      //
      const float nb_carac_per_line_f( nb_carac_per_line ) ;
      const float nb_carac_per_column_f( nb_carac_per_column ) ;

      //
      const float cell_width( font.cell_width );
      const float cell_height( font.cell_height );

      //
      const float cell_width_normal( font.coef_width / cell_width );
      const float cell_height_normal( font.coef_height / cell_height );

      //
      const float half_size( size / 2.f );

      //
      text_heap_t::vertex_expander triangles( text_heap.complete_primitive( primitive_type::triangles ) );

      //
      QMatrix4x4 text_model;
      text_model.setToIdentity();
      text_model.translate( pos );
      text_model.rotate( degree_angle_3d.x(), 1.f, 0, 0 );
      text_model.rotate( degree_angle_3d.y(), 0, 1.f, 0 );
      text_model.rotate( degree_angle_3d.z(), 0, 0, 1.f );

      //
      triangles.set_uniform( "text_model", text_model );
      triangles.set_uniform( "text_color", color );

      //
      const float position_decal(
        ( align_h == quick_text_horizontal_align::left ) ?
        0.f :
        ( ( align_h == quick_text_horizontal_align::center ) ?
          -( float( t_length ) / 2.f ) :
          -float( t_length )
        )
      );

      //
      for ( uint i = 0 ; i < t_length ; ++i )
      {
        const float column( ( float( i ) + position_decal ) * font.coef_extra_spacement );

        const QVector2D vertex_up_left( column * size, half_size );
        const QVector2D vertex_up_right( column * size + size, half_size );
        const QVector2D vertex_down_right( column * size + size, -half_size );
        const QVector2D vertex_down_left( column * size, -half_size );

        const uint character_base( static_cast<uint>( t[i] ) );
        if ( ( character_base < font.start_char ) || ( character_base > font.end_char ) )
          continue;

        //
        const uint character( character_base - font.start_char );

        //
        const float character_mod( character % nb_carac_per_line );
        const float character_div( character / nb_carac_per_line );

        //
        const float uv_x( ( font.row_read ? character_div : character_mod ) / nb_carac_per_line_f );
        const float uv_y( ( font.row_read ? character_mod : character_div ) / nb_carac_per_column_f );

        //
        const QVector2D uv_up_left( uv_x, uv_y );
        const QVector2D uv_up_right( uv_x + cell_width_normal, uv_y );
        const QVector2D uv_down_right( uv_x + cell_width_normal, uv_y + cell_height_normal );
        const QVector2D uv_down_left( uv_x, uv_y + cell_height_normal );

        triangles.push( vertex_up_left, uv_up_left );
        triangles.push( vertex_down_left, uv_down_left );
        triangles.push( vertex_up_right, uv_up_right );

        triangles.push( vertex_down_right, uv_down_right );
        triangles.push( vertex_up_right, uv_up_right );
        triangles.push( vertex_down_left, uv_down_left );
      }
    }

  }
}
