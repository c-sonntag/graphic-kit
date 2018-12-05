#include <gk/opengl/model/from_object.hpp>

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

      std::unique_ptr<from_object::shared_glsl> from_object::glsl_vertex;
      std::unique_ptr<from_object::shared_glsl> from_object::glsl_vertex_uv;
      std::unique_ptr<from_object::shared_glsl> from_object::glsl_vertex_uv_norm;

      // ---- ----

#define byteof_vec( vec ) ( vec.size()* sizeof( std::remove_reference<decltype( vec )>::type::value_type ) )
#define get_vec( el_true, el2_false ) ( el.is_finalized ? el_true : el2_false )
#define get_vec_byte( el_true, el2_false ) ( byteof_vec( get_vec( el_true, el2_false ) ) )

      // ---- ----

      from_object::shared_glsl& from_object::return_shared( const encoder::object::element& el )
      {
        const bool have_vertices( get_vec( el.finalized_vertices, el.vertices ).size() > 0 );
        const bool have_texture2d_coords( get_vec( el.finalized_texture2d_coords, el.texture2d_coords ).size() > 0 );
        const bool have_normals( get_vec( el.finalized_normals, el.normals ).size() > 0 );

        if( have_vertices && have_texture2d_coords && have_normals )
        {
          if( !glsl_vertex_uv_norm )
          {
            glsl_vertex_uv_norm = std::make_unique<from_object::shared_glsl>(
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
            glsl_vertex_uv = std::make_unique<from_object::shared_glsl>(
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
            glsl_vertex = std::make_unique<from_object::shared_glsl>(
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

      /**
       * @see http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-7-model-loading/
       */
      from_object::from_object( const encoder::object::element& el ) :
        glsl( return_shared( el ) ),
        buffer( raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw )
      {

        //
        if( !el.is_finalized && el.have_indices )
          throw std::runtime_error( "Need finalized indices obj" );

        //
        ulong totalbytes( 0 );
        totalbytes += get_vec_byte( el.finalized_vertices, el.vertices );
        totalbytes += get_vec_byte( el.finalized_texture2d_coords, el.texture2d_coords );
        totalbytes += get_vec_byte( el.finalized_normals, el.normals );
        totalbytes += get_vec_byte( el.finalized_texture3d_coords, el.texture3d_coords );

        // Reserve size
        buffer.bind_and_send( totalbytes, nullptr );

        //
        vao.bind();

        //
        ulong bytes( 0 );
        uint layout_index( 0 );
        static const auto append_vec(
          [&]( uint vec_value_nb, auto& el_true, auto& el_false )
          {
            using value_type = typename std::remove_reference<decltype( el_true )>::type::value_type;
            const auto& vec( get_vec( el_true, el_false ) );
            if( !vec.empty() )
            {
              vao.attrib( buffer, layout_index, vec_value_nb, raiigl::data_type::Float, false, 0, bytes );
              buffer.bind_and_update( vec, bytes );
              bytes += vec.size() * sizeof( value_type );
            }
            ++layout_index;
          }
        );

        //
        append_vec( 3, el.finalized_vertices, el.vertices );                  // layout 1
        append_vec( 2, el.finalized_texture2d_coords, el.texture2d_coords );  // layout 2
        append_vec( 3, el.finalized_normals, el.normals );                    // layout 3
        append_vec( 3, el.finalized_texture3d_coords, el.texture3d_coords );  // layout 4

        //
        vao.unbind();
        buffer.unbind();

        //
        vertices_count = uint( get_vec( el.finalized_vertices, el.vertices ).size() );

      }

      //   from_object::from_object( const encoder::object& o, std::unique_ptr<raiigl::texture> _texture_up ) :
      //     from_object( o )
      //   { set_texture( std::move( _texture_up ) );  }


      // ---- ----

      void from_object::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        //
        glsl.program.use();
        vao.bind();

        //
        glsl.uniform_mvp.set( camera * model );
        glsl.uniform_use_texture.set( use_texture );

        //
        if( use_texture )
        {
          //texture_sp->texture.bind();
          glsl.sampler_texture.set( texture_sp->texture_num );
          texture_sp->texture.bind_on_texture( texture_sp->texture_num );
        }
        else
          glsl.uniform_color.set( color );

        //
        gl.draw_arrays( raiigl::primitive_type::Triangles, 0, vertices_count );

        //
        if( use_texture )
          texture_sp->texture.unbind();

        //
        vao.unbind();
        glsl.program.unuse();
      }

      // ---- ----

      void from_object::set_color( const glm::vec4& _color )
      { use_texture = 0; color = _color; }

      void from_object::set_texture( std::shared_ptr<gk::opengl::texture_binded> _texture_sp )
      {
        texture_sp = std::move( _texture_sp );
        if( texture_sp )
          use_texture = 1;
      }

    }
  }
}

