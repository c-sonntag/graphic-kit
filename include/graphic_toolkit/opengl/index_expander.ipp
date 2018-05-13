#pragma once

#include <graphic_toolkit/opengl/index_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {

    inline index_expander_property::index_expander_property( primitive_type _primitive, size_t _vertex_start, size_t _index_start ) :
      abstract_expander_property( std::move( _primitive ) ),
      vertex_start( std::move( _vertex_start ) ), index_start( std::move( _index_start ) ),
      vertex_count( 0 ), index_count( 0 )
    { }

    inline void index_expander_property::gl_draw( const expander_property_support &, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & ) const
    {
      if ( ( vertex_count > 0 ) && ( index_count > 0 ) )
        gl.glDrawElementsBaseVertex(
          GLenum( primitive ),
          GLint( index_count ),
          index_buffer::gl_indice_type,
          reinterpret_cast<const GLvoid *>( index_start * sizeof( index_buffer::indice_size ) ),
          GLint( vertex_start )
        );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline index_expander<TListTypes...>::index_expander( expander_property_support & _expander_support, vertex_buffer_t & _vertices, index_buffer_t & _indices, primitive_type _primitive ) :
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
    inline void index_expander<TListTypes...>::push_vertex( Args && ... args )
    {
      vertices.rows.emplace_back( args... );
    }

    template<typename  ... TListTypes>
    inline void index_expander<TListTypes...>::push_index( index_buffer_t::indice_t i )
    {
      indices.indices.emplace_back( i );
    }

    template<typename  ... TListTypes>
    inline void index_expander<TListTypes...>::reserve_vertices( size_t n )
    {
      vertices.rows.reserve( expander_property_up->vertex_start + n );
    }

    template<typename  ... TListTypes>
    inline void index_expander<TListTypes...>::reserve_indices( size_t n )
    {
      indices.indices.reserve( expander_property_up->index_start + n );
    }

  }
}
