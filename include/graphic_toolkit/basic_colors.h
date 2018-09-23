#pragma once

#include <graphic_toolkit/color.h>

namespace graphic_toolkit {
  namespace basic_colors {

    constexpr color::rgb black( 0.f, 0.f, 0.f );
    constexpr color::rgb white( 1.f, 1.f, 1.f );

    constexpr color::rgb intense_red( 1.f, 0.f, 0.f );
    constexpr color::rgb intense_green( 0.f, 1.f, 0.f );
    constexpr color::rgb intense_blue( 0.f, 0.f, 1.f );

    /** @see Hue on https://www.w3schools.com/color/color_picker.asp */

    constexpr color::rgb orange( 255, 191, 0 );
    constexpr color::rgb yellow( 255, 255, 0 );
    constexpr color::rgb nvidia_green_1( 153, 255, 51 );
    constexpr color::rgb nvidia_green_2( 204, 255, 51 );

  }
}

