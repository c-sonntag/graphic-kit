#pragma once

#include <gk/vectors.hpp>

#include <gk/color/list.hpp>

#include <gk/opengl/model/abstract.hpp>
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
      struct from_vectors : public abstract
      {
       public:
        struct shared_glsl
        {
          raiigl::program program;
          raiigl::uniform_variable uniform_mvp{ program, "uniform_mvp" };
          raiigl::uniform_variable uniform_color{ program, "uniform_color" };
          raiigl::uniform_variable uniform_texture_mode{ program, "uniform_texture_mode" };
          raiigl::uniform_variable uniform_sampler_texture2d{ program, "uniform_sampler_texture2d" };

          inline shared_glsl( raiigl::program _program ) :
            program( std::move( _program ) )
          {}
        };

       protected:
        static shared_glsl& return_glsl( const bool have_vertices, const bool have_texture2d_coords, const bool have_normals );
        static std::unique_ptr<shared_glsl> glsl_vertex, glsl_vertex_uv, glsl_vertex_uv_norm;

       public:
        shared_glsl& glsl;
        raiigl::vertex_array vao;

       protected:
        enum class texture_mode : uint { color=0, rgb=1, rgba=2, r=3 };

       protected:
        glm::vec4 color{ gk::color::list::nvidia_green_2.normalize(), 1.f };
        texture_mode tex2d_mode = texture_mode::color;
        std::shared_ptr<gk::opengl::texture_binded> texture_sp;

        //std::unique_ptr<raiigl::texture> texture_up;
        //raiigl::textures_num texture_num = raiigl::textures_num::None;

       protected:
        raiigl::buffer buffer;
        uint vertices_count = 0;
        uint vertices_first = 0;

        //std::unique_ptr<raiigl::buffer> index_buffer;

       public:
        from_vectors( const vec::vertices_vector& v, const vec::texture2d_coords_vector& uv2d = {}, const vec::normals_vector& norm = {} );

       public:
        virtual void paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model ) override;

       public:
        virtual void set_color( const glm::vec4& _color );
        virtual void set_texture( std::shared_ptr<gk::opengl::texture_binded>& _texture_sp );

      };


    }
  }
}

