#pragma once

#include <graphic_toolkit/opengl/quick_text.h>

#include <graphic_toolkit/opengl/quick_program.h>

#include <QFileInfo>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    inline QImage quick_text::get_text_texture( uint font_id )
    {
      if ( font_id >= fonts_list_text_count )
        throw std::runtime_error( "[graphic_toolkit::quick_text] Can't find this font_id" );

      //
      QString texture_bmp_file_path(
        ":/graphic-toolkit/opengl/quick_text/512x512-" + QString( fonts_list_text[font_id] ) + ".bmp"
      );

      //
      QFileInfo file_info( texture_bmp_file_path );
      const bool exist_file { file_info.exists() && file_info.isFile() };

      //
      if ( !exist_file )
        throw std::runtime_error( "[graphic_toolkit::quick_text] Imposible to locate text texture : " + texture_bmp_file_path.toStdString() );

      //
      return QImage( texture_bmp_file_path ).mirrored();
    }

    // ---- ----

    inline quick_text::quick_text( quick_text::font_name font_id ) :
      text_program_up( quick_program( ":/graphic-toolkit/opengl/quick_text/text_triangles.vert", ":/graphic-toolkit/opengl/quick_text/text_triangles.frag" ) ),
      text_texture( get_text_texture( uint( font_id ) ) ),
      text_heap(
        attrib_pointer( 0, 2, GL_FLOAT, true ), // vertex
        attrib_pointer( 1, 2, GL_FLOAT, true ) // uv
      )
    {
      //
      text_texture.setMinMagFilters(
        QOpenGLTexture::Filter::Nearest,
        QOpenGLTexture::Filter::Linear
      );
      text_texture.setWrapMode( QOpenGLTexture::Repeat );

    }

    // ---- ----

    inline void quick_text::draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view )
    {
      if ( !text_heap.is_init() )
        text_heap.init_buffer();

      //
      text_program_up->bind();
      text_program_up->setUniformValue( "mvp_matrix", projection_view );
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

    inline bool quick_text::empty() const {
        return text_heap.empty();
    }

    // ---- ----

    inline void quick_text::add( const std::string & t, const float size, const QVector3D & pos, const QVector3D & angle )
    {
      const uint t_length( t.size() );

      text_heap_t::vertex_expander triangles( text_heap.complete_primitive( primitive_type::triangles ) );

      for ( uint i_uint = 0 ; i_uint < t_length ; ++i_uint )
      {
        const float i( i_uint );

        QVector2D vertex_up_left( pos.x() + i * size, pos.y() + size );
        QVector2D vertex_up_right( pos.x() + i * size + size, pos.y() + size );
        QVector2D vertex_down_right( pos.x() + i * size + size, pos.y() );
        QVector2D vertex_down_left( pos.x() + i * size, pos.y() );

        const char character( t[i_uint] );
        const float uv_x( ( character % 16 ) / 16.0f );
        const float uv_y( ( character / 16 ) / 16.0f );

        QVector2D uv_up_left( uv_x, uv_y );
        QVector2D uv_up_right( uv_x + 1.0f / 16.0f, uv_y );
        QVector2D uv_down_right( uv_x + 1.0f / 16.0f, ( uv_y + 1.0f / 16.0f ) );
        QVector2D uv_down_left( uv_x, ( uv_y + 1.0f / 16.0f ) );

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

