#pragma once

#include <gk/opengl/model/abstract.hpp>
#include <gk/decoder/object.hpp>
#include <gk/opengl/texture.hpp>

#include <raiigl/vertex_array.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <memory>

namespace gk {
  namespace opengl {
    namespace model {

      /**
       * @see http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-7-model-loading/
       */
      struct from_object : public abstract
      {
       public:
        struct shared_glsl
        {
          raiigl::program program;
          raiigl::uniform_variable uniform_mvp{ program, "uniform_mvp" };
          raiigl::uniform_variable uniform_color{ program, "uniform_color" };
          raiigl::uniform_variable uniform_use_texture{ program, "uniform_use_texture" };
          raiigl::uniform_variable sampler_texture{ program, "sampler_texture" };

          inline shared_glsl( raiigl::program _program ) :
            program( std::move( _program ) )
          {}
        };

       protected:
        static shared_glsl& return_shared( const decoder::object::element& el );
        static std::unique_ptr<shared_glsl> glsl_vertex, glsl_vertex_uv, glsl_vertex_uv_norm;

       public:
        shared_glsl& glsl;
        raiigl::vertex_array vao;

       protected:
        glm::vec4 color;
        int use_texture = 0;
        std::shared_ptr<gk::opengl::texture_binded> texture_sp;

        //std::unique_ptr<raiigl::texture> texture_up;
        //raiigl::textures_num texture_num = raiigl::textures_num::None;

       protected:
        raiigl::buffer buffer;
        uint vertices_count = 0;

        //std::unique_ptr<raiigl::buffer> index_buffer;

       public:
        from_object( const decoder::object::element& el );
        //from_object( const decoder::object& o, std::unique_ptr<raiigl::texture> _texture_up );

       public:
        void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) override;

       public:
        void set_color( const glm::vec4& _color );
        void set_texture( std::shared_ptr<gk::opengl::texture_binded> _texture_sp );

      };


    }
  }
}

