#pragma once

#include <gtools/opengl/model/abstract.hpp>

#include <raiigl/vertex_array.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <memory>

namespace gtools {
  namespace opengl {
    namespace model {

      struct cube : public abstract_by_color
      {
       protected:
        struct gl_support
        {
          raiigl::program glsl;
          raiigl::buffer buffer;
          raiigl::vertex_array vao;
          raiigl::uniform_variable uniform_mvp;
          raiigl::uniform_variable uniform_color;
          gl_support();
          void paint( raiigl::gl330& gl, const glm::mat4& mvp );
          void set_color( const glm::vec4& color );
        };

       protected:
        static std::unique_ptr<gl_support> gl_support_up;

       public:
        cube();

       public:
        void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) override;
        void set_color( const glm::vec4& color ) override;
      };


    }
  }
}


