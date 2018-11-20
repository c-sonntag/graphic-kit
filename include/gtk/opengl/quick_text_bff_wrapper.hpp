#pragma once

#include <gtk/types.hpp>

#include <raiigl/texture.hpp>

#include <glm/vec2.hpp>

#include <string>

namespace gtk {
  namespace opengl {

    struct quick_text_bbf_wrapper
    {
      struct bbf_font
      {
       public:
        uint start_char = 0, end_char = 0;
        uint width = 0, height = 0;
        uint cell_width = 0, cell_height = 0;
        uint y_offset = 0;
        uint row_pitch = 0;

       public:
        float row_factor = 0, col_factor = 0;
        glm::vec2 normal_scale;

       public:
        uchar characters_width[256] {0};

       public:
        bbf_font( const std::string & bbf_data_font, raiigl::texture & texture );
      };

     public:
      raiigl::texture texture;
      const bbf_font font;

     public:
      quick_text_bbf_wrapper( const std::string & bbf_data_font );
    };

  }
}
