#pragma once

#include <gtk/types.hpp>
#include <gtk/render/abstract.hpp>

#include <bits/move.h>

namespace gtk {
  namespace render {

    class data_texture : public abstract
    {
     public:
      const uint size;
      /** @todo and current rendering (opengl/vulkan) object */

     public:
      inline data_texture( const uint _size ) :
        size( std::move( _size ) ) {}
    };

  }
}


