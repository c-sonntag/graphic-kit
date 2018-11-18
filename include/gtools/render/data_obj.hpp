#pragma once

#include <gtools/types.hpp>
#include <gtools/render/abstract.hpp>

#include <bits/move.h>

namespace gtools {
  namespace render {

    class data_obj : public abstract
    {
     public:
      const uint size;
      /** @todo and current rendering (opengl/vulkan) object */

     public:
      inline data_obj( const uint _size ) :
        size( std::move( _size ) ) {}
    };

  }
}


