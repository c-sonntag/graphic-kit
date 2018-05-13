#pragma once

#include <graphic_toolkit/opengl/vertex_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {



    void vertex_expander_property::gl_draw( const expander_property_support &, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & ) const
    {
      if ( count > 0 )
        gl.glDrawArrays( GLenum( primitive ), GLint( start ), GLint( count ) );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::vertex_expander( expander_property_support & _expander_support, vertex_buffer_t & _vertices, primitive_type _primitive ) :
      abstract_expander( _expander_support ),
      vertices( _vertices ),
      vertex_property( std::make_unique<vertex_expander_property>( _primitive, vertices.rows.size() ) )
    {
      expander_support.lock();
    }

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::~vertex_expander()
    {
      vertex_property->count = vertices.rows.size() - vertex_property->start;
      expander_support.unlock( std::move( vertex_property ) );
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
      vertices.rows.reserve( vertex_property->start + n );
    }

  }
}

