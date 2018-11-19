#include <gtools/opengl/model/cube_box.hpp>

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

      std::unique_ptr<cube_box::gl_support> cube_box::gl_support_up;

      // ---- ----

      static structures_pack::vertex cube_box_vertices[]
      {
        {{ -1.f, +1.f, +1.f }}, {{ +1.f, +1.f, +1.f }}, // Front
        {{ +1.f, +1.f, +1.f }}, {{ +1.f, -1.f, +1.f }},
        {{ +1.f, -1.f, +1.f }}, {{ -1.f, -1.f, +1.f }},
        {{ -1.f, -1.f, +1.f }}, {{ -1.f, +1.f, +1.f }},
        {{ -1.f, +1.f, -1.f }}, {{ +1.f, +1.f, -1.f }}, // Back
        {{ +1.f, +1.f, -1.f }}, {{ +1.f, -1.f, -1.f }},
        {{ +1.f, -1.f, -1.f }}, {{ -1.f, -1.f, -1.f }},
        {{ -1.f, -1.f, -1.f }}, {{ -1.f, +1.f, -1.f }},
        {{ -1.f, +1.f, +1.f }}, {{ -1.f, +1.f, -1.f }}, // Mid
        {{ +1.f, +1.f, +1.f }}, {{ +1.f, +1.f, -1.f }},
        {{ +1.f, -1.f, +1.f }}, {{ +1.f, -1.f, -1.f }},
        {{ -1.f, -1.f, +1.f }}, {{ -1.f, -1.f, -1.f }},
      };
      static uint cube_box_vertices_count( sizeof( cube_box_vertices ) / sizeof( structures_pack::vertex ) );


      cube_box::gl_support::gl_support() :
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
        buffer.bind_and_send( cube_box_vertices );
        vao.unbind();
        buffer.unbind();
      }

      void cube_box::gl_support::paint( raiigl::gl330& gl, const glm::mat4& mvp )
      {
        glsl.use();
        vao.bind();

        uniform_mvp.set( mvp );

        gl.draw_arrays( raiigl::primitive_type::Lines, 0, cube_box_vertices_count );

        vao.unbind();
        glsl.unuse();
      }

      // ---- ----

      cube_box::cube_box()
      {
        if( !gl_support_up )
          gl_support_up = std::make_unique<gl_support>();
      }

      void cube_box::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        if( gl_support_up )
          gl_support_up->paint( gl, camera * model );
      }

    }
  }
}


