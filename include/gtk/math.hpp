#pragma once

#include <cmath>

namespace gtk {
  namespace math {

    static constexpr float pi = float(M_PI);
    static constexpr float pi_tour = float(2.0 * M_PI);
    static constexpr float demi_pi = float(M_PI_2);
    static constexpr float quart_pi = float(M_PI_4);


    template<typename T>
    inline bool is_pow_of( T base, T number )
    {
      // @see https://www.geeksforgeeks.org/check-if-a-number-is-power-of-another-number/
      // logarithm function to calculate value
      const double r( std::log( number ) / std::log( base ) );
      return std::floor( r ) == r;
    }

  }
}


