#pragma once

#include <graphic_toolkit/opengl/primitives_heap.h>

#include <graphic_toolkit/opengl/vertex_expander.h>
#include <graphic_toolkit/opengl/index_expander.h>

namespace graphic_toolkit {
  namespace opengl {

    template< typename  ... TListTypes >
    template< typename... Args >
    inline primitives_heap<TListTypes...>::primitives_heap( Args ... attributes ) :
      initialized( false ), is_busy( false ),
      attrib_pointers( attributes... )
    { }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program )
    {
      if ( !initialized )
        return;
      for ( draw_buffer & db : expanders )
        db.draw( *this, gl, program );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::auto_draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program )
    {
      attrib_array_enable_all();
      draw( gl, program );
      attrib_array_disable_all();
    }

    // ---- ----

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::attrib_array_enable_all( QOpenGLFunctions_3_3_Core & gl )
    {
      for ( const attrib_pointer_by_offset & aps : attrib_pointers )
        gl.glEnableVertexAttribArray( aps.gl_location );
    }

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::attrib_array_disable_all( QOpenGLFunctions_3_3_Core & gl )
    {
      for ( const attrib_pointer_by_offset & aps : attrib_pointers )
        gl.glDisableVertexAttribArray( aps.gl_location );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::lock()
    {
      if ( is_busy )
        throw std::runtime_error( "primitives_heap is already locked" );
      is_busy = true;
    }

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::unlock( typename expander::property_up_t property_up )
    {
      expanders.emplace_back( std::move( property_up ) );
      is_busy = false;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline primitives_heap<TListTypes...>::vertex_expander complete_primitive( primitive_type primitive ) {
        return vertex_expander( *this, primitive );
    }

    template< typename  ... TListTypes >
    inline primitives_heap<TListTypes...>::index_expander complete_indexed_primitive( primitive_type primitive ) {
        return index_expander( *this, primitive );
    }

    // ---- ---- ---- ----






  }
}
