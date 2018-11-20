#pragma once

#include <glm/vec3.hpp>

#include <bits/move.h>

namespace structures_pack {

  struct vertex
  {
   public:
    glm::vec3 vec;

   public:
    inline vertex( glm::vec3 _vec ) :
      vec( std::move( _vec ) ) {}
  };

}
