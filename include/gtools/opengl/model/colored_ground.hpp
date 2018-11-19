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

      struct colored_ground : public abstract
      {
       protected:
        struct gl_support
        {
         protected:
          raiigl::program glsl;
          raiigl::buffer buffer;
          raiigl::vertex_array vao;
          raiigl::uniform_variable uniform_mvp;
          raiigl::uniform_variable uniform_time;
          float seconds_elpased = 0.f;
         public:
          gl_support();
          void paint( raiigl::gl330& gl, const glm::mat4& mvp );
          void anime( const gtools::time& t );
        };

       protected:
        static std::unique_ptr<gl_support> gl_support_up;

       public:
        colored_ground();

       public:
        void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) override;
        void anime( const gtools::time& t ) override;
      };


    }
  }
}

