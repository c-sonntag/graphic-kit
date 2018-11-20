#pragma once

#include <gtk/color/rgba.hpp>
#include <gtk/color/hsl.hpp>


namespace gtk {
  namespace gui {

    struct color_palette
    {
      glm::vec4 reset_button { color::rgb( 255, 214, 168 ).normalize(), 1.f };

    };

    extern color_palette color_palette_default;

  }
}

