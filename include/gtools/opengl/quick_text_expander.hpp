#pragma once

#include <gtools/types.hpp>
#include <gtools/color/list.hpp>
#include <gtools/opengl/quick_text.hpp>

#include <glm/vec3.hpp>

#include <string>

namespace gtools {
  namespace opengl {

    struct text_expander
    {
     public:
      enum horizontal_align : uint
      { left, center, right };

      enum vertical_align : uint
      { top, middle, bottom };

     public:
      std::string text;
      float normal_size = 0.1f;
      glm::vec3 pos { 0.f, 0.f, 0.f };
      glm::vec3 degree_angle_3d {0.f, 0.f, 0.f};
      glm::vec3 color {color::list::white};
      horizontal_align align_h = left;
      vertical_align align_v = middle;
      bool text_auto_width = true;
      float coef_extra_spacement = 1.f;

     protected:
      friend quick_text;
      quick_text & qt;
      text_expander( quick_text & _qt );
      text_expander( quick_text & _qt, std::string t );

     public:
      // Enable move.
      text_expander( text_expander && ) = default;
      // Disable copy from lvalue.
      text_expander( const text_expander & ) = delete;
      text_expander & operator=( const text_expander & ) = delete;

     public:
      ~text_expander();

     public:
      float get_base_width() const;
      float get_base_height() const;

     public:
      float get_normal_width() const;
      float get_normal_height() const;

     public:
      float get_normal_width_for_current_size() const;
      float get_normal_height_for_current_size() const;

     protected:
      void push_text();
    };
  }
}
