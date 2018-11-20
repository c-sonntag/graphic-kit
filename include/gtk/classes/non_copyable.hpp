#pragma once

#include <bits/move.h>

namespace gtk {
  namespace classes {

    struct non_copyable
    {
      // Default constructors
      non_copyable() = default;
      virtual ~non_copyable() = default;

      // Enable move.
      non_copyable( non_copyable && ) = default;

      // Disable copy from lvalue.
      non_copyable( const non_copyable & ) = delete;
      non_copyable & operator=( const non_copyable & ) = delete;

    };

  }
}
