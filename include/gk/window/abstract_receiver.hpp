#pragma once

#include <glm/vec2.hpp>

namespace gk {
  namespace window {

    struct abstract_receiver
    {
     public:
      virtual ~abstract_receiver() = default;

     public:
      virtual void reshape( const glm::uvec2& resolution ) = 0;
    };

  }
}


