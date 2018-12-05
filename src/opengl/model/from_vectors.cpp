#include <gk/opengl/model/from_vectors.hpp>

#include <gk/decoder/object.hpp>

#include <gk/structures_pack/vertex.hpp>
#include <gk/structures_pack/vertex_uv.hpp>
#include <gk/structures_pack/vertex_uv_norm.hpp>

#include <gk/opengl/quick_program.hpp>

#include <erc/package_id.h>
#include <erc/inventory_package.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>

namespace gk {
  namespace opengl {
    namespace model {

      static const erc::package_id package_id( "opengl" );

      std::unique_ptr<from_vectors::shared_glsl> from_vectors::glsl_vertex;
      std::unique_ptr<from_vectors::shared_glsl> from_vectors::glsl_vertex_uv;
      std::unique_ptr<from_vectors::shared_glsl> from_vectors::glsl_vertex_uv_norm;

      // ---- ----

      from_vectors::shared_glsl& from_vectors::return_glsl( const bool have_vertices, const bool have_texture2d_coords, const bool have_normals )
      {
        if( have_vertices && have_texture2d_coords && have_normals )
        {
          if( !glsl_vertex_uv_norm )
          {
            glsl_vertex_uv_norm = std::make_unique<from_vectors::shared_glsl>(
              gk::opengl::quick_program::open_from_local_erc(
                package_id.from( "glsl/vertex/basic_vertex_uv_norm.vert" ),
                package_id.from( "glsl/fragment/object.frag" )
              )
            );
          }
          return *glsl_vertex_uv_norm;
        }
        else if( have_vertices && have_texture2d_coords )
        {
          if( !glsl_vertex_uv )
          {
            glsl_vertex_uv = std::make_unique<from_vectors::shared_glsl>(
              gk::opengl::quick_program::open_from_local_erc(
                package_id.from( "glsl/vertex/basic_vertex_uv.vert" ),
                package_id.from( "glsl/fragment/object.frag" )
              )
            );
          }
          return *glsl_vertex_uv;
        }
        else
        {
          if( !glsl_vertex )
          {
            glsl_vertex = std::make_unique<from_vectors::shared_glsl>(
              gk::opengl::quick_program::open_from_local_erc(
                package_id.from( "glsl/vertex/basic_vertex.vert" ),
                package_id.from( "glsl/fragment/object.frag" )
              )
            );
          }
          return *glsl_vertex;
        }
      }

      // ---- ----

      // #define get_vec( name ) ( el.finalized ? el_true : el2_false )
      // #define get_vec_byte( el_true, el2_false ) ( vec::bytesize( get_vec( el_true, el2_false ) ) )

      /**
       * @see http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-7-model-loading/
       */
      from_vectors::from_vectors( const vec::vertices_vector& v, const vec::texture2d_coords_vector& uv2d, const vec::normals_vector& norm ) :
        glsl( return_glsl( !v.empty(), !uv2d.empty(), !norm.empty() ) ),
        buffer( raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw )
      {

        //
        ulong totalbytes( 0 );
        totalbytes += vec::bytesize( v );
        totalbytes += vec::bytesize( uv2d );
        totalbytes += vec::bytesize( norm );
        //totalbytes += vec::bytesize( uv3d );

        // Reserve size
        buffer.bind_and_send( totalbytes, nullptr );

        //
        vao.bind();

        //
        ulong bytes_offset( 0 );
        uint layout_index( 0 );
        static const auto append_vec(
          [&]( uint vec_value_nb, const auto& vec )
          {
            if( !vec.empty() )
            {
              vao.attrib( buffer, layout_index, vec_value_nb, raiigl::data_type::Float, false, 0, bytes_offset );
              buffer.bind_and_update( vec, bytes_offset );
              bytes_offset += vec::bytesize( vec );
            }
            ++layout_index;
          }
        );

        //
        append_vec( 3, v );     // layout 0
        append_vec( 2, uv2d );  // layout 1
        append_vec( 3, norm );  // layout 2
        // append_vec( 3, uv3d );  // layout 3

        //
        vao.unbind();
        buffer.unbind();

        //
        vertices_count = uint( v.size() );
        vertices_first = 0;
      }

      //   from_vectors::from_vectors( const decoder::object& o, std::unique_ptr<raiigl::texture> _texture_up ) :
      //     from_vectors( o )
      //   { set_texture( std::move( _texture_up ) );  }


      // ---- ----

      void from_vectors::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        //
        glsl.program.use();
        vao.bind();

        //
        glsl.uniform_mvp.set( camera * model );
        glsl.uniform_texture_mode.set( int(tex2d_mode) );

        //
        if( tex2d_mode == texture_mode::color )
          glsl.uniform_color.set( color );
        else
        {
          //texture_sp->bind();
          //texture_sp->texture.bind_on_texture( texture_sp->texture_num );
          //texture_sp->texture.bind();
          //texture_sp->bind();
          texture_sp->bind_on_texture( texture_sp->texture_num );
          //glsl.uniform_sampler_texture2d.set( texture_sp->texture_num );
          //glsl.uniform_sampler_texture2d.set( raiigl::textures_num::Texture00 );
          glsl.uniform_sampler_texture2d.set( texture_sp->texture_num );
        }

        //
        gl.draw_arrays( raiigl::primitive_type::Triangles, vertices_first, vertices_count );

        //
        // if( use_texture )
        //   texture_sp->texture.unbind();

        //
        vao.unbind();
        glsl.program.unuse();
      }

      // ---- ----

      void from_vectors::set_color( const glm::vec4& _color )
      { tex2d_mode = texture_mode::color; color = _color; }

      void from_vectors::set_texture( std::shared_ptr<gk::opengl::texture_binded>& _texture_sp )
      {
        texture_sp = _texture_sp;
        if( texture_sp )
        {
          if( texture_sp->last_pixel_format() == raiigl::pixel_format::RGB )
            tex2d_mode = texture_mode::rgb;
          else if( texture_sp->last_pixel_format() == raiigl::pixel_format::RGBA )
            tex2d_mode = texture_mode::rgba;
          else if( texture_sp->last_pixel_format() == raiigl::pixel_format::Red )
            tex2d_mode = texture_mode::r;
        }
      }

    }
  }
}

