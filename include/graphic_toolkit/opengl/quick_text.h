#pragma once
#ifndef graphic_toolkit_opengl_quick_text_h
#define graphic_toolkit_opengl_quick_text_h

#include <graphic_toolkit/types.h>
#include <graphic_toolkit/opengl/primitives_heap.h>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>

namespace graphic_toolkit {
  namespace opengl {

    static constexpr const char * fonts_list_text[] { "Times_New_Roman", };
    static constexpr uint fonts_list_text_count( sizeof( fonts_list_text ) / sizeof( const char * ) );

    /** @see http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-11-2d-text/ */
    struct quick_text
    {
     public:
      enum font_name : uint
      { TimesNewRoman = 0 };

     protected:
      std::unique_ptr<QOpenGLShaderProgram> text_program_up;
      QOpenGLTexture text_texture;

     protected:
      // Text position, UV Mapping
      using text_heap_t =  primitives_heap<QVector2D, QVector2D>;
      text_heap_t text_heap;

     protected:
      static QImage get_text_texture( uint font_id );

     public:
      quick_text( font_name font_id );

     public:
      void draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view );
      bool empty() const;

     public:
      void add( const std::string & t, const float size, const QVector3D & pos, const QVector3D & angle );


    };

  }
}

#include <graphic_toolkit/opengl/quick_text.ipp>
#endif

