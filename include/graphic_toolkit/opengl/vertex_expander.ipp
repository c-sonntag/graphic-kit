#pragma once

#include <graphic_toolkit/opengl/vertex_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    inline vertex_expander_property::vertex_expander_property( primitive_type _primitive, size_t _start ) :
      abstract_expander_property( std::move( _primitive ) ),
      start( std::move( _start ) ),
      count( 0 )
    { }

    inline void vertex_expander_property::gl_draw( const expander_property_support &, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & ) const
    {
      if ( count > 0 )
        gl.glDrawArrays( GLenum( primitive ), GLint( start ), GLint( count ) );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::vertex_expander( expander_property_support & _expander_support, vertex_buffer_t & _vertices, primitive_type _primitive ) :
      abstract_expander(
        _expander_support,
        std::make_unique<vertex_expander_property>( _primitive, _vertices.rows.size() )
      ),
      vertices( _vertices )
    { }

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::~vertex_expander()
    {
      expander_property_up->count = vertices.rows.size() - expander_property_up->start;
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    template< class... Args >
    inline void vertex_expander<TListTypes...>::push( Args && ... args )
    {
      vertices.rows.emplace_back( args... );
    }

    template<typename  ... TListTypes>
    inline void vertex_expander<TListTypes...>::reserve( size_t n )
    {
      vertices.rows.reserve( expander_property_up->start + n );
    }

  }
}

