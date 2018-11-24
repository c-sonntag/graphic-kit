#pragma once

#include <gk/math.hpp>

namespace gk {
  namespace math {


    template<typename T>
    inline bool is_pow_of( T base, T number )
    {
      // @see https://www.geeksforgeeks.org/check-if-a-number-is-power-of-another-number/
      // logarithm function to calculate value
      const double r( std::log(number) / std::log(base) );
      return std::floor( r ) == r;
    }

    // ---- ---- ---- ----

    inline in_circle_property::in_circle_property( const float x, const float y, const float x_center, const float y_center ) :
      base_x( x - x_center ), base_y( y - y_center ),
      rayon( std::sqrt( base_x * base_x + base_y * base_y ) + 0.0001f ),
      part( std::atan2( base_y, base_x ) ) {}


    // ---- ---- ---- ----


    inline void dist_xyz( glm::vec3& position, const float decal )
    {
      in_circle_property in_circle_xz( position.x, position.z );
      in_circle_property in_circle_xy( position.x, position.y );
      position.x = std::cos( in_circle_xz.part ) * ( in_circle_xz.rayon + decal );
      position.z = std::sin( in_circle_xz.part ) * ( in_circle_xz.rayon + decal );
      position.y = std::sin( in_circle_xy.part ) * ( in_circle_xy.rayon + decal );
    }

    inline void rot_xz( glm::vec3& position, const float decal )
    {
      in_circle_property in_circle( position.x, position.z );
      position.x = std::cos( in_circle.part + decal ) * in_circle.rayon;
      position.z = std::sin( in_circle.part + decal ) * in_circle.rayon;
    }

    inline void rot_zy( glm::vec3& position, const float decal )
    {
      in_circle_property in_circle( position.z, position.y );
      position.z = std::cos( in_circle.part + decal ) * in_circle.rayon;
      position.y = std::sin( in_circle.part + decal ) * in_circle.rayon;
    }

    inline void rot_y( glm::vec3& position, const float decal )
    {
      // in_circle_property in_circle_z( position.z, position.y );
      // in_circle_property in_circle_x( position.x, position.y );
      //
      // position.x = std::cos( in_circle_x.part + decal ) * in_circle_x.rayon;
      // position.z = std::cos( in_circle_z.part + decal ) * in_circle_z.rayon;
      position.y += decal * 10.f;
      //position.y += decal;
    }


  }
}


