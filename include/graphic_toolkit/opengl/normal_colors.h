#pragma once

#include <graphic_toolkit/types.h>

#include <glm/vec3.hpp>


namespace graphic_toolkit {
  namespace opengl {
    namespace normal_colors {

      constexpr inline glm::vec3 normalize( uchar r, uchar g, uchar b ) { return glm::vec3( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f ); }

      constexpr glm::vec3 black( 0.f, 0.f, 0.f );
      constexpr glm::vec3 white( 1.f, 1.f, 1.f );

      constexpr glm::vec3 intense_red( 1.f, 0.f, 0.f );
      constexpr glm::vec3 intense_green( 0.f, 1.f, 0.f );
      constexpr glm::vec3 intense_blue( 0.f, 0.f, 1.f );


      /** @see Hue on https://www.w3schools.com/colors/colors_picker.asp */

      constexpr glm::vec3 orange( normalize( 255, 191, 0 ) );
      constexpr glm::vec3 yellow( normalize( 255, 255, 0 ) );
      constexpr glm::vec3 nvidia_green_1( normalize( 153, 255, 51 ) );
      constexpr glm::vec3 nvidia_green_2( normalize( 204, 255, 51 ) );


    }
  }
}

