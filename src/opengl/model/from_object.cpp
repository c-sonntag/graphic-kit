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

      from_object::shared_glsl& from_object::return_shared( const encoder::object::element& el )
      {
        const bool have_vertices( el.vertices.size() > 0 );
        const bool have_texture2d_coords( el.texture2d_coords.size() > 0 );
        const bool have_normals( el.normals.size() > 0 );
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
#define byteof_vec( vec ) ( vec.size() * sizeof( std::remove_reference<decltype( vec )>::type::value_type ) )
#define get_vec( el_true, el2_false ) ( el.is_finalized ? el_true : el2_false )
#define get_vec_byte( el_true, el2_false ) ( byteof_vec( get_vec( el_true, el2_false ) ) )

        //
        ulong totalbytes( 0 );
        totalbytes += get_vec_byte( el.finalized_vertices, el.vertices );
        totalbytes += get_vec_byte( el.finalized_normals, el.normals );
        totalbytes += get_vec_byte( el.finalized_texture2d_coords, el.texture2d_coords );
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
            //using value_type = typename decltype( el_true )::value_type;
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
        append_vec( 3, el.finalized_vertices, el.vertices );
        append_vec( 3, el.finalized_normals, el.normals );
        append_vec( 2, el.finalized_texture2d_coords, el.texture2d_coords );
        append_vec( 3, el.finalized_texture3d_coords, el.texture3d_coords );

        vao.unbind();
        buffer.unbind();

        vertices_count = uint( get_vec( el.finalized_vertices, el.vertices ).size() );

        //        const bool have_vertices( el.vertices.size() > 0 );
        //        const bool have_texture2d_coords( el.texture2d_coords.size() > 0 );
        //        const bool have_normals( el.normals.size() > 0 );

        //        if( have_vertices && have_texture2d_coords && have_normals )
        //        {
        //          // if(
        //          //   ( el.vertices.size() != el.texture2d_coords.size() )
        //          //   || ( el.vertices.size() != el.normals.size() )
        //          // )
        //          // {
        //          //   std::stringstream ss;
        //          //   ss << "[gk::opengl::model::from_object] Need same quantity of"
        //          //      << " vertices(" + std::to_string( el.vertices.size() ) + ")"
        //          //      << " texture2d_coords(" + std::to_string( el.texture2d_coords.size() ) + ")"
        //          //      << " normals(" + std::to_string( el.normals.size() ) + ")";
        //          //   throw std::runtime_error( ss.str() );
        //          // }

        //          //
        //          std::vector<structures_pack::vertex_uv_norm> pack;
        //          for( uint i( 0 ); i<el.vertex_indices.size(); ++i ) {
        //            // Get the indices of its attributes
        //            const uint vertex_index( el.vertex_indices[i] );
        //            const uint uv_index( el.tex_coord_indices[i] );
        //            const uint normal_index( el.normal_indices[i] );
        //            // Get the attributes thanks to the index
        //            pack.emplace_back(
        //              el.vertices[vertex_index - ( ( vertex_index == 0 ) ? 0 : 1 )],
        //              el.texture2d_coords[uv_index - ( ( uv_index == 0 ) ? 0 : 1 )],
        //              el.normals[normal_index - ( ( normal_index == 0 ) ? 0 : 1 )]
        //            );
        //          }
        //          vao.bind();
        //          vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv_norm ), offsetof( structures_pack::vertex_uv_norm, vec ) );
        //          vao.attrib( buffer, 1, 2, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv_norm ), offsetof( structures_pack::vertex_uv_norm, uv ) );
        //          vao.attrib( buffer, 2, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv_norm ), offsetof( structures_pack::vertex_uv_norm, norm ) );
        //          buffer.bind_and_send( pack );
        //          vao.unbind();
        //          buffer.unbind();
        //        }
        //        else if( have_vertices && have_texture2d_coords )
        //        {
        //          // if( el.vertices.size() != el.texture2d_coords.size() )
        //          // {
        //          //   std::stringstream ss;
        //          //   ss << "[gk::opengl::model::from_object] Need same quantity of"
        //          //      << " vertices(" + std::to_string( el.vertices.size() ) + ")"
        //          //      << " texture2d_coords(" + std::to_string( el.texture2d_coords.size() ) + ")";
        //          //   throw std::runtime_error( ss.str() );
        //          // }

        //          //
        //          std::vector<structures_pack::vertex_uv> pack;
        //          for( uint i( 0 ); i<el.vertex_indices.size(); ++i ) {
        //            // Get the indices of its attributes
        //            const uint vertex_index( el.vertex_indices[i] );
        //            const uint uv_index( el.tex_coord_indices[i] );
        //            // Get the attributes thanks to the index
        //            pack.emplace_back(
        //              el.vertices[vertex_index - ( ( vertex_index == 0 ) ? 0 : 1 )],
        //              el.texture2d_coords[uv_index - ( ( uv_index == 0 ) ? 0 : 1 )]
        //            );
        //          }
        //          vao.bind();
        //          vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv ), offsetof( structures_pack::vertex_uv, vec ) );
        //          vao.attrib( buffer, 1, 2, raiigl::data_type::Float, false, sizeof( structures_pack::vertex_uv ), offsetof( structures_pack::vertex_uv, uv ) );
        //          buffer.bind_and_send( pack );
        //          vao.unbind();
        //          buffer.unbind();
        //        }
        //        else
        //        {
        //          //
        //          std::vector<structures_pack::vertex> pack;
        //          for( uint i( 0 ); i<el.vertex_indices.size(); ++i ) {
        //            const uint vertex_index( el.vertex_indices[i] );
        //            pack.emplace_back( el.vertices[vertex_index - ( ( vertex_index == 0 ) ? 0 : 1 )] );
        //          }
        //          vao.bind();
        //          vao.attrib( buffer, 0, 3, raiigl::data_type::Float, false, sizeof( structures_pack::vertex ), offsetof( structures_pack::vertex, vec ) );
        //          buffer.bind_and_send( pack );
        //          vao.unbind();
        //          buffer.unbind();
        //        }


        // if( el.have_indices )
        // {}
        // else
        // {}

      }

      //   from_object::from_object( const encoder::object& o, std::unique_ptr<raiigl::texture> _texture_up ) :
      //     from_object( o )
      //   { set_texture( std::move( _texture_up ) );  }


      // ---- ----

      void from_object::paint( raiigl::gl330& gl, const matrices::base_camera& camera, const glm::mat4& model )
      {
        glsl.program.use();
        vao.bind();

        glsl.uniform_mvp.set( camera * model );
        glsl.uniform_use_texture.set( use_texture );

        if( use_texture )
          glsl.sampler_texture.set( texture_sp->texture_num );
        else
          glsl.uniform_color.set( color );

        gl.draw_arrays( raiigl::primitive_type::Triangles, 0, vertices_count );

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

        //texture_up = std::move( _texture_up );
        //if( texture_up )
        //{
        //  use_texture = 1;
        //  if( texture_num == raiigl::textures_num::None )
        //    texture_num = glsl.program.new_texture_index();
        //  texture_up->bind_on_texture( texture_num );
        //}
      }

    }
  }
}

