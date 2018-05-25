#pragma once


#include <graphic_toolkit/types.h>
#include <graphic_toolkit/opengl/quick_text.h>

#include <QVector3D>

#include <string>

namespace graphic_toolkit {
  namespace opengl {

    struct text_expander
    {
     public:
      enum  horizontal_align : uint
      { left, center, right };

      enum vertical_align : uint
      { top, middle, bottom };

     public:
      std::string text;
      float size = 15.f;
      QVector3D pos = QVector3D( 0.f, 0.f, 0.f );
      QVector3D degree_angle_3d = QVector3D( 0.f, 0.f, 0.f );
      QVector3D color = normal_colors::white;
      horizontal_align align_h = left;
      vertical_align align_v = top;
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
      float get_width() const;
      float get_height() const;

     protected:
      void push_text();
    };

  }
}

