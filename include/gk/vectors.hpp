#pragma once

#include <gk/types.hpp>

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gk {
  namespace vec {

    template<typename T>
    __forceinline uint bytesize( const std::vector<T>& vec )
    { return vec.size() * sizeof( T ); }

    using vertices_vector =  std::vector<glm::vec3>;

    using normals_vector =  std::vector<glm::vec3>;

    using texture2d_coords_vector = std::vector<glm::vec2>;
    using texture3d_coords_vector = std::vector<glm::vec3>;

    using index_vector = std::vector<uint>;

  }
}
