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

    // ---- ----

    enum class text_horizontal_align : uint
    { left, center, right };

    enum class text_vertical_align : uint
    { top, middle, bottom };

    // ---- ----

    struct text_expander;

    // ---- ----

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

     protected:
      bool busy = false;

     protected:
      void check_not_busy() const;

     public:
      void draw( QOpenGLFunctions_3_3_Core & gl, const QMatrix4x4 & projection_view );
      bool empty() const;

     public:
      void reset();

     protected:
      friend text_expander;
      void lock();
      void unlock();

     public:
      inline text_expander complete_text();
      inline text_expander complete_text( std::string t );
    };

    // ---- ---- ---- ----

    struct text_expander
    {
     public:
      std::string text;
      float size = 15.f;
      QVector3D pos = QVector3D( 0.f, 0.f, 0.f );
      QVector3D degree_angle_3d = QVector3D( 0.f, 0.f, 0.f );
      QVector3D color = normal_colors::white;
      text_horizontal_align align_h = text_horizontal_align::left;
      text_vertical_align align_v = text_vertical_align::top;
      bool text_auto_width = true;

     protected:
      friend quick_text;
      quick_text & qt;
      inline text_expander( quick_text & _qt );
      inline text_expander( quick_text & _qt, std::string t );

     public:
      // Enable move.
      text_expander( text_expander && ) = default;
      // Disable copy from lvalue.
      text_expander( const text_expander & ) = delete;
      text_expander & operator=( const text_expander & ) = delete;

     public:
      ~text_expander();

     public:
      float get_width() const;
      float get_height() const;

     protected:
      void push_text();
    };

    // ---- ---- ---- ----

    inline text_expander::text_expander( quick_text & _qt ) :
      qt( _qt )
    {}

    inline text_expander::text_expander( quick_text & _qt, std::string t ) :
      text( std::move( t ) ), qt( _qt )
    {}

    // ---- ----

    inline text_expander quick_text::complete_text()
    {
      return text_expander( *this );
    }

    inline text_expander quick_text::complete_text( std::string t )
    {
      return text_expander( *this, std::move( t ) );
    }

  }
}

#include <graphic_toolkit/opengl/quick_text.ipp>
#endif

