#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <bits/move.h>

namespace structures_pack {

  struct vertex_uv
  {
   public:
    glm::vec3 vec;
    glm::vec2 uv;

   public:
    inline vertex_uv( glm::vec3 _vec, glm::vec2 _uv ) :
      vec( std::move( _vec ) ), uv( std::move( _uv ) ) {}

    //explicit inline vertex_uv( glm::vec3 && _vec, glm::vec2 && _uv ) :
    //  vec( std::move( _vec ) ), uv( std::move( _uv ) ) {}
    //explicit inline vertex_uv( const glm::vec3 & _vec, const glm::vec2 & _uv ) :
    //  vec( std::move( _vec ) ), uv( std::move( _uv ) ) {}
  };

}
