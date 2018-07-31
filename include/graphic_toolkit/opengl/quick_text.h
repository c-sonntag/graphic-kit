#pragma once

#include <graphic_toolkit/types.h>
#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/normal_colors.h>
#include <graphic_toolkit/opengl/quick_text_bff_wrapper.h>

#include <raiigl/gl330.hpp>
#include <raiigl/program.hpp>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>

namespace graphic_toolkit {
  namespace opengl {

    enum class quick_text_fonts : uint
    {
      CalibriLight_512,
      Calibri_512_rgb,
      CalibriLight_1024,
    };

    // ---- ----

    struct text_expander;
    struct font_allocated_assoc;

    // ---- ----

    /** @see http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-11-2d-text/ */
    struct quick_text
    {
     public:
      using font_name_t = quick_text_fonts;
      using text_expander_t = text_expander;

     protected:
      //std::shared_ptr<const quick_text_bbf_wrapper> bff_font;
      const quick_text_fonts font_id;
      font_allocated_assoc * faa_p = nullptr;
      quick_text_bbf_wrapper * bff_font_p = nullptr;

     public:
      inline quick_text_bbf_wrapper * get_font() { return bff_font_p;}

     protected:
      // Text position, UV Mapping
      using text_heap_t =  primitives_heap<glm::vec2, glm::vec2>;
      text_heap_t text_heap;

     public:
      quick_text( quick_text_fonts font_id );
      ~quick_text();

     public:
      void init_gl();

     protected:
      bool busy = false;

     protected:
      void check_not_busy() const;

     public:
      void draw( raiigl::gl330 & gl, const QMatrix4x4 & projection_view );
      bool empty() const;

     public:
      void reset();

     protected:
      friend text_expander;
      void lock();
      void unlock();

     public:
      text_expander complete_text();
      text_expander complete_text( std::string t );
    };

  }
}
