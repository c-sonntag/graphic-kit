#include <gk/opengl/model/sphere.hpp>

#include <gk/types.hpp>
#include <gk/structures_pack/vertex_uv_norm.hpp>
#include <gk/opengl/quick_program.hpp>

#include <erc/package_id.h>
#include <erc/inventory_package.h>

#include <vector>

namespace gk {
  namespace opengl {
    namespace model {

      static const erc::package_id package_id( "opengl" );

      std::unique_ptr<sphere::gl_support> sphere::gl_support_up;

      // ---- ----

      sphere::gl_support::gl_support() :
        glsl(
          gk::opengl::quick_program::open_from_local_erc(
            package_id.from( "glsl/vertex/basic_vertex.vert" ),
            package_id.from( "glsl/fragment/colored_by_uniform_color.frag" )
          )
        ),
        buffer( raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw ),
        index_buffer( raiigl::buffer_type::ElementArray, raiigl::buffer_usage::StaticDraw ),
        uniform_mvp( glsl, "uniform_mvp" ),
        uniform_color( glsl, "uniform_color" )
      {


        std::vector<structures_pack::vertex_uv_norm> sphere;
        sphere.reserve( sphere_index_count );
        {
          const float radius = 0.95f;
          const glm::vec3 decal( 0.f, -0.0f, 0.f );

          for( float latNumber( 0.f ); latNumber <= latitudeBands; ++latNumber )
          {
            float theta = latNumber * float(M_PI) / latitudeBands;
            float sinTheta = std::sin( theta );
            float cosTheta = std::cos( theta );
            for( float longNumber( 0.f ); longNumber <= longitudeBands; ++longNumber )
            {
              const float phi = longNumber * 2.f * float(M_PI) / longitudeBands;
              const float sinPhi = std::sin( phi );
              const float cosPhi = std::cos( phi );

              const float x = cosPhi * sinTheta;
              const float y = cosTheta;
              const float z = sinPhi * sinTheta;
              const float u = 1.f - ( longNumber / longitudeBands );
              const float v = 1.f - ( latNumber / latitudeBands );

              sphere.emplace_back(
                ( decal + glm::vec3( radius * x, radius * y, radius * z ) ), // vertices
                glm::vec2( u, v ), // texcoords
                glm::vec3( x, y, z ) // normals
              );

            }
          }
        }

        using index_pack = glm::highp_uvec3;
        std::vector<index_pack> sphere_indexes;
        sphere_indexes.reserve( sphere_index_count * 2 );
        {
          for( float latNumber( 0.f ); latNumber < latitudeBands; ++latNumber )
            for( float longNumber( 0.f ); longNumber < longitudeBands; ++longNumber )
            {
              const GLuint first = GLuint( ( latNumber * ( longitudeBands + 1 ) ) + longNumber );
              const GLuint second = GLuint( first + longitudeBands + 1 );

              sphere_indexes.emplace_back( first, second, first + 1 );
              sphere_indexes.emplace_back( second, second + 1, first + 1 );
            }
        }

        //
        count = uint( sphere_indexes.size() * 3 );

        //
        vao.bind();
        vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv_norm ), offsetof( structures_pack::vertex_uv_norm, vec ) );

        buffer.bind_and_send( sphere );
        index_buffer.bind_and_send( sphere_indexes );

        vao.unbind();
        buffer.unbind();
        index_buffer.unbind();
      }

      void sphere::gl_support::paint( raiigl::gl330& gl, const glm::mat4& mvp )
      {
        glsl.use();
        vao.bind();

        uniform_mvp.set( mvp );

        gl.draw_elements( raiigl::primitive_type::Triangles, count );

        vao.unbind();
        glsl.unuse();
      }

      void sphere::gl_support::set_color( const glm::vec4& color )
      {
        glsl.use();
        uniform_color.set( color );
        glsl.unuse();
      }

      // ---- ----

      sphere::sphere()
      {
        if( !gl_support_up )
          gl_support_up = std::make_unique<gl_support>();
      }

      void sphere::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        if( gl_support_up )
          gl_support_up->paint( gl, camera * model );
      }

      void sphere::set_color( const glm::vec4& color )
      {
        if( gl_support_up )
          gl_support_up->set_color( color );
      }

    }
  }
}


