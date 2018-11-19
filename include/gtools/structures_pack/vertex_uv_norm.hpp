#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <bits/move.h>

namespace structures_pack {

  struct vertex_uv_norm
  {
   public:
    glm::vec3 vec;
    glm::vec2 uv;
    glm::vec3 norm;

   public:
    inline vertex_uv_norm( glm::vec3 _vec, glm::vec2 _uv, glm::vec3 _norm ) :
      vec( std::move( _vec ) ), uv( std::move( _uv ) ), norm( std::move( _norm ) ) {}
  };

}
