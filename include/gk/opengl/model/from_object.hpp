#pragma once

#include <gk/opengl/model/from_vectors.hpp>

#include <gk/decoder/object.hpp>

namespace gk {
  namespace opengl {
    namespace model {

      struct from_object : public from_vectors
      {
       public:
        from_object( const decoder::object::element::finalized& o ) :
          from_vectors( o.vertices, o.texture2d_coords, o.normals )
        { }
      };

    }
  }
}

