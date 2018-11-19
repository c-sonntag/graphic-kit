#pragma once

#include <glm/vec3.hpp>

namespace gtools {
  namespace random {

    float positive_normal_value( const float coef = 1.f );
    glm::vec3 normal_position( const float coef = 1.f );

  }
}
