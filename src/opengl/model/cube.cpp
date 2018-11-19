#include <gtools/opengl/model/cube.hpp>

#include <gtools/types.hpp>
#include <gtools/structures_pack/vertex.hpp>
#include <gtools/opengl/quick_program.hpp>

#include <erc/package_id.h>
#include <erc/inventory_package.h>

#include <chrono>
#include <iostream>

namespace gtools {
  namespace opengl {
    namespace model {

      static const erc::package_id package_id( "opengl" );

      std::unique_ptr<cube::gl_support> cube::gl_support_up;

      // ---- ----

      static structures_pack::vertex cube_vertices[]
      {
        {{ -1.0f, -1.0f, -1.0f }},
        {{ -1.0f, -1.0f, 1.0f }},
        {{ -1.0f, 1.0f, 1.0f }},
        {{ 1.0f, 1.0f, -1.0f }},
        {{ -1.0f, -1.0f, -1.0f }},
        {{ -1.0f, 1.0f, -1.0f }},
        {{ 1.0f, -1.0f, 1.0f }},
        {{ -1.0f, -1.0f, -1.0f }},
        {{ 1.0f, -1.0f, -1.0f }},
        {{ 1.0f, 1.0f, -1.0f }},
        {{ 1.0f, -1.0f, -1.0f }},
        {{ -1.0f, -1.0f, -1.0f }},
        {{ -1.0f, -1.0f, -1.0f }},
        {{ -1.0f, 1.0f, 1.0f }},
        {{ -1.0f, 1.0f, -1.0f }},
        {{ 1.0f, -1.0f, 1.0f }},
        {{ -1.0f, -1.0f, 1.0f }},
        {{ -1.0f, -1.0f, -1.0f }},
        {{ -1.0f, 1.0f, 1.0f }},
        {{ -1.0f, -1.0f, 1.0f }},
        {{ 1.0f, -1.0f, 1.0f }},
        {{ 1.0f, 1.0f, 1.0f }},
        {{ 1.0f, -1.0f, -1.0f }},
        {{ 1.0f, 1.0f, -1.0f }},
        {{ 1.0f, -1.0f, -1.0f, }},
        {{ 1.0f, 1.0f, 1.0f }},
        {{ 1.0f, -1.0f, 1.0f }},
        {{ 1.0f, 1.0f, 1.0f }},
        {{ 1.0f, 1.0f, -1.0f }},
        {{ -1.0f, 1.0f, -1.0f }},
        {{ 1.0f, 1.0f, 1.0f }},
        {{ -1.0f, 1.0f, -1.0f }},
        {{ -1.0f, 1.0f, 1.0f }},
        {{ 1.0f, 1.0f, 1.0f }},
        {{ -1.0f, 1.0f, 1.0f }},
        {{ 1.0f, -1.0f, 1.0f }},
      };
      static uint cube_vertices_count( sizeof( cube_vertices ) / sizeof( structures_pack::vertex ) );


      cube::gl_support::gl_support() :
        glsl(
          gtools::opengl::quick_program::open_from_local_erc(
            package_id.from( "glsl/vertex/basic_vertex.vert" ),
            package_id.from( "glsl/fragment/colored_by_uniform_color.frag" )
          )
        ),
        buffer( raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw ),
        uniform_mvp( glsl, "uniform_mvp" ),
        uniform_color( glsl, "uniform_color" )
      {
        vao.bind();
        vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex ), offsetof( structures_pack::vertex, vec ) );
        buffer.bind_and_send( cube_vertices );
        vao.unbind();
        buffer.unbind();
      }

      void cube::gl_support::paint( raiigl::gl330& gl, const glm::mat4& mvp )
      {
        glsl.use();
        vao.bind();

        uniform_mvp.set( mvp );

        gl.draw_arrays( raiigl::primitive_type::Triangles, 0, cube_vertices_count );

        vao.unbind();
        glsl.unuse();
      }

      void cube::gl_support::set_color( const glm::vec4& color )
      {
        glsl.use();
        uniform_color.set( color );
        glsl.unuse();
      }

      // ---- ----

      cube::cube()
      {
        if( !gl_support_up )
          gl_support_up = std::make_unique<gl_support>();
      }

      void cube::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        if( gl_support_up )
          gl_support_up->paint( gl, camera * model );
      }

      void cube::set_color( const glm::vec4& color )
      {
        if( gl_support_up )
          gl_support_up->set_color( color );
      }

    }
  }
}


