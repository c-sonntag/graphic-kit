#pragma once

#include <gk/types.hpp>
#include <gk/opengl/model/abstract.hpp>

#include <raiigl/vertex_array.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <memory>

namespace gk {
  namespace opengl {
    namespace model {

      struct sphere : public abstract_by_color
      {
       public:
        static constexpr float latitudeBands = 30.f;
        static constexpr float longitudeBands = 30.f;
        static constexpr uint sphere_index_count{ uint( latitudeBands * longitudeBands ) };

       protected:
        struct gl_support
        {
         protected:
          raiigl::program glsl;
          raiigl::buffer buffer;
          raiigl::buffer index_buffer;
          raiigl::vertex_array vao;
          raiigl::uniform_variable uniform_mvp;
          raiigl::uniform_variable uniform_color;
          uint count = 0;
         public:
          gl_support();
          void paint( raiigl::gl330& gl, const glm::mat4& mvp );
          void set_color( const glm::vec4& color );
        };

       protected:
        static std::unique_ptr<gl_support> gl_support_up;

       public:
        sphere();

       public:
        void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) override;
        void set_color( const glm::vec4& color ) override;
      };


    }
  }
}

