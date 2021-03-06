#pragma once

#include <gk/time.hpp>
#include <gk/matrices/base_camera.hpp>

#include <raiigl/gl330.hpp>

#include <glm/glm.hpp>

namespace gk {
  namespace opengl {
    namespace model {

      struct abstract
      {
       public:
        virtual ~abstract() = default;

       public:
        virtual void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) = 0;
        virtual void anime( const gk::time& ) { /*nothing */ }
      };

      struct abstract_by_color : public abstract
      {
       public:
        virtual ~abstract_by_color() = default;

       public:
        virtual void set_color( const glm::vec4& color ) = 0;
      };

    }
  }
}


