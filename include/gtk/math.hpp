#pragma once

#include <cmath>
#include <glm/vec3.hpp>

namespace gtk {
  namespace math {

    static constexpr float pi = float(M_PI);
    static constexpr float pi_tour = float(2.0 * M_PI);
    static constexpr float demi_pi = float(M_PI_2);
    static constexpr float quart_pi = float(M_PI_4);

    // ---- ---- ---- ----

    template<typename T>
    inline bool is_pow_of( T base, T number );

    // ---- ---- ---- ----

    struct in_circle_property
    {
      float base_x, base_y;
      float rayon;
      float part;
      in_circle_property( const float x, const float y, const float x_center = 0.f, const float y_center = 0.f );
    };

    void dist_xyz( glm::vec3& position, const float x );
    void rot_xz( glm::vec3& position, const float x );
    void rot_zy( glm::vec3& position, const float x );

    // ---- ---- ---- ----


  }
}

// ---- ---- ---- ----

#include <gtk/math.ipp>
