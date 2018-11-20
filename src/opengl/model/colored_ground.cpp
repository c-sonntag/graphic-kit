#include <gtk/opengl/model/colored_ground.hpp>

#include <gtk/structures_pack/vertex_uv.hpp>

#include <erc/package_id.h>
#include <erc/inventory_package.h>

#include <gtk/opengl/quick_program.hpp>


namespace gtk {
  namespace opengl {
    namespace model {

      static const erc::package_id package_id( "opengl" );

      std::unique_ptr<colored_ground::gl_support> colored_ground::gl_support_up;

      // ---- ----

      static structures_pack::vertex_uv colored_ground_vertices[4]
      {
        {{ -1.f, 0.f, +1.f }, { 0.f, 0.f }},
        {{ +1.f, 0.f, +1.f }, { 1.f, 0.f }},
        {{ -1.f, 0.f, -1.f }, { 0.f, 1.f }},
        {{ +1.f, 0.f, -1.f }, { 1.f, 1.f }},
      };

      colored_ground::gl_support::gl_support() :
        glsl(
          gtk::opengl::quick_program::open_from_local_erc(
            package_id.from( "glsl/vertex/basic_vertex_uv.vert" ),
            package_id.from( "glsl/fragment/uv_colored_by_time.frag" )
          )
        ),
        buffer( raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw ),
        uniform_mvp( glsl, "uniform_mvp" ),
        uniform_time( glsl, "uniform_time" )
      {
        vao.bind();
        vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv ), offsetof( structures_pack::vertex_uv, vec ) );
        vao.attrib( buffer, 1, 2, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv ), offsetof( structures_pack::vertex_uv, uv ) );
        buffer.bind_and_send( colored_ground_vertices );
        vao.unbind();
        buffer.unbind();
      }

      void colored_ground::gl_support::paint( raiigl::gl330& gl, const glm::mat4& mvp )
      {
        glsl.use();
        vao.bind();

        gl.disable( raiigl::gl_capabilities::CullFace );

        // gl.depth_func( raiigl::deth_func_type::Less );

        uniform_mvp.set( mvp );
        uniform_time.set( seconds_elpased );

        gl.draw_arrays( raiigl::primitive_type::TriangleStrip, 0, 4 );

        gl.enable( raiigl::gl_capabilities::CullFace );

        vao.unbind();
        glsl.unuse();
      }

      void colored_ground::gl_support::anime( const gtk::time& t )
      {
        seconds_elpased = t.seconds_elpased;
      }

      // ---- ----

      colored_ground::colored_ground()
      {
        if( !gl_support_up )
          gl_support_up = std::make_unique<gl_support>();
      }

      void colored_ground::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        if( gl_support_up )
          gl_support_up->paint( gl, camera * model );
      }

      void colored_ground::anime( const gtk::time& t )
      {
        if( gl_support_up )
          gl_support_up->anime( t );
      }

    }
  }
}


