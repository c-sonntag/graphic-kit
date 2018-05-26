#pragma once

#include <graphic_toolkit/types.h>

#include <QVector2D>
#include <QOpenGLTexture>
#include <QFile>

namespace graphic_toolkit {
  namespace opengl {


    struct quick_text_bbf_wrapper
    {
      struct bbf_font
      {
       public:
        uint start_char, end_char;
        uint width, height;
        uint cell_width, cell_height;
        uint y_offset;
        uint row_pitch;

       public:
        float row_factor, col_factor;
        QVector2D normal_scale;

       public:
        uchar characters_width[256];

       public:
        bbf_font( QFile & in, QOpenGLTexture & texture );
      };

     public:
      QOpenGLTexture texture;
      const bbf_font font;

     public:
      quick_text_bbf_wrapper( QFile & in );
    };

  }
}

