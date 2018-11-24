#pragma once

#include <gk/opengl/primitives_heap/index_expander.hpp>

#include <stdexcept>

namespace gk {
  namespace opengl {

    inline index_expander_property::index_expander_property( raiigl::primitive_type _primitive, size_t _vertex_start, size_t _index_start ) :
      abstract_expander_property( std::move( _primitive ) ),
      vertex_start( std::move( _vertex_start ) ), index_start( std::move( _index_start ) ),
      vertex_count( 0 ), index_count( 0 )
    { }

    inline void index_expander_property::gl_draw( const abstract_expander_property_support &, raiigl::gl330 & gl, raiigl::program & ) const
    {
      if ( indice_is_global && ( index_count > 0 ) )
        gl.draw_elements(
          primitive,
          index_count,
          reinterpret_cast<const void *>( index_start * index_buffer::indice_size ),
          index_buffer::indice_type
        );
      else if ( ( vertex_count > 0 ) && ( index_count > 0 ) )
        gl.draw_elements_base_vertex(
          primitive,
          index_count,
          vertex_start,
          reinterpret_cast<void *>( index_start * index_buffer::indice_size ),
          index_buffer::indice_type
        );
    }

    inline void index_expander_property::gl_draw( const abstract_expander_property_support & aeps, raiigl::gl430 & gl, raiigl::program & proc ) const
    {
      gl_draw( aeps, gl, proc );
    }

    // ---- ---- ---- ----

    inline void index_expander_property::gl_multiple_instance_draw( const abstract_expander_property_support & aeps, raiigl::gl430 & gl, raiigl::program & proc, const uint nb_instance ) const
    {
      gl.draw_elements_instanced_base_vertex_base_instance(
        primitive,
        index_count,
        nb_instance,
        0,
        vertex_start,
        reinterpret_cast<const void *>( index_start * index_buffer::indice_size ),
        index_buffer::indice_type
      );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline index_expander<TListTypes...>::index_expander( abstract_expander_property_support & _expander_support, vertex_buffer_t & _vertices, index_buffer_t & _indices, raiigl::primitive_type _primitive ) :
      abstract_expander(
        _expander_support,
        std::make_unique<index_expander_property>( std::move( _primitive ), _vertices.rows.size(), _indices.indices.size() )
      ),
      vertices( _vertices ),
      indices( _indices ),
      vertex_start_decal( expander_property_up->vertex_start )
    {  }

    template<typename  ... TListTypes>
    inline index_expander<TListTypes...>::~index_expander()
    {
      expander_property_up->vertex_count = vertices.rows.size() - expander_property_up->vertex_start;
      expander_property_up->index_count = indices.indices.size() - expander_property_up->index_start;
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void index_expander<TListTypes...>::push_vertex( Args && ... args )
    { vertices.rows.emplace_back( args... ); }

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void index_expander<TListTypes...>::push_vertex( const Args && ... args )
    { vertices.rows.emplace_back( args... ); }

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void index_expander<TListTypes...>::push_vertex( const Args & ... args )
    { vertices.rows.emplace_back( args... ); }

    // ---- ----

    template<typename  ... TListTypes>
    __forceinline void index_expander<TListTypes...>::push_index( index_buffer_t::indice_t i )
    { indices.indices.emplace_back( i ); }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    __forceinline void index_expander<TListTypes...>::set_global_indice( bool v )
    { expander_property_up->indice_is_global = v; }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    __forceinline void index_expander<TListTypes...>::reserve_vertices( size_t n )
    { vertices.rows.reserve( expander_property_up->vertex_start + n ); }

    template<typename  ... TListTypes>
    __forceinline void index_expander<TListTypes...>::reserve_indices( size_t n )
    { indices.indices.reserve( expander_property_up->index_start + n ); }

  }
}

