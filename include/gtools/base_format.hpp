#pragma once

#include <gtools/types.hpp>

#include <cmath>

namespace gtools {
  namespace base_format {

    template <typename T>
    inline bool is_pow_of( T base, T number )
    {
      // @see https://www.geeksforgeeks.org/check-if-a-number-is-power-of-another-number/
      // logarithm function to calculate value
      const double r( std::log( number ) / std::log( base ) );
      return ( std::floor( r ) == r );
    }

  }
}
