#pragma once

#include <glm/vec2.hpp>

namespace gtools {
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


