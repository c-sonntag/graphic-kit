#pragma once
#ifndef graphic_toolkit_opengl_quick_text_h
#define graphic_toolkit_opengl_quick_text_h

#include <graphic_toolkit/types.h>
#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/normal_colors.h>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>

namespace graphic_toolkit {
  namespace opengl {

    enum class quick_text_font_name : uint
    {
      TimesNewRoman_ascii_extented = 0,
      Calibri_ascii_extented = 1,
    };

    enum class quick_text_horizontal_align : uint
    {
      left, center, right
    };

    /** @see http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-11-2d-text/ */
    struct quick_text
    {
     public:
      using font_name_t = quick_text_font_name;

     protected:
      struct font_property
      {
        std::string font_basename;
        uint width, height;
        uint cell_width, cell_height;
        uint start_char, end_char;
        float coef_width, coef_height;
        float coef_extra_spacement;
        bool row_read;  /**< (not tested) If the char are top-to-bottom and not left-to-right */
      };

     protected:
      static const font_property fonts_list[];
      static const uint fonts_list_count;

     private:
      static font_property get_font( uint font_id );
      static QImage get_text_texture( const font_property & font );

     public:
      const font_property font;

     protected:
      quick_program text_program;
      std::unique_ptr<QOpenGLTexture> text_texture_up = nullptr;

     protected:
      // Text position, UV Mapping
      using text_heap_t =  primitives_heap<QVector2D, QVector2D>;
      text_heap_t text_heap;

     public:
      quick_text( quick_text_font_name font_id );

     public:
      void init_gl();

     public:
      void draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view );
      bool empty() const;

     public:
      void reset();

     public:
      void add( const std::string & t, const float size, const QVector3D & pos, const QVector3D & degree_angle_3d, const QVector3D & color = normal_colors::white, quick_text_horizontal_align align_h = quick_text_horizontal_align::left );


    };

  }
}

#include <graphic_toolkit/opengl/quick_text.ipp>
#endif

