#pragma once

#include <gtools/time.hpp>
#include <gtools/matrices/base_camera.hpp>

#include <raiigl/gl330.hpp>

#include <glm/glm.hpp>

namespace gtools {
  namespace opengl {
    namespace model {

      struct abstract
      {
       public:
        virtual ~abstract() = default;

       public:
        virtual void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) = 0;
        virtual void anime( const gtools::time& ) { /*nothing */ }
      };

    }
  }
}


