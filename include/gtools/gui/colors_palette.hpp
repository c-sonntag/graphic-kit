#pragma once

#include <gtools/color/rgba.hpp>
#include <gtools/color/hsl.hpp>


namespace gtools {
  namespace gui {

    struct color_palette
    {
      glm::vec4 reset_button { color::rgb( 255, 214, 168 ).normalize(), 1.f };

    };

    extern color_palette color_palette_default;

  }
}

