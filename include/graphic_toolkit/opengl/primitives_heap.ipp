#pragma once

#include <graphic_toolkit/opengl/primitives_heap.h>

#include <graphic_toolkit/opengl/vertex_expander.h>
#include <graphic_toolkit/opengl/index_expander.h>

namespace graphic_toolkit {
  namespace opengl {

    template< typename  ... TListTypes >
    template< typename... Args >
    inline primitives_heap<TListTypes...>::primitives_heap( Args ... attributes ) :
      attrib_pointers( attributes... ),
      initialized( false ),  is_busy( false ),
      property_support( *this )
    { }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::gl_attrib_pointer( QOpenGLFunctions_3_3_Core & gl )
    {
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.glVertexAttribPointer(
          apo.gl_location,
          apo.gl_tuple_size,
          apo.gl_type,
          apo.gl_normalized,
          vertices_t::row_size,
          reinterpret_cast<const GLvoid *>( apo.opposed_offset ) /**< std::tuple have data inverted order */
        );
    }


    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program )
    {
      if ( !initialized )
        return;

      //
      gl_attrib_pointer( gl );

      //
      for ( const abstract_expander_property_up_t & aep_up : expanders_properties )
        aep_up->draw( property_support, gl, program );
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
    inline primitives_heap<TListTypes...>::expander_property_support_inherited::expander_property_support_inherited( primitives_heap_t & _heap ) :
      heap( _heap )
    {}

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::expander_property_support_inherited::lock()
    {
      if ( heap.is_busy )
        throw std::runtime_error( "primitives_heap is already locked" );
      heap.is_busy = true;
    }

    template< typename  ... TListTypes >
    void primitives_heap<TListTypes...>::expander_property_support_inherited::unlock( abstract_expander_property_up_t property_up )
    {
      heap.expanders_properties.emplace_back( std::move( property_up ) );
      heap.is_busy = false;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::vertex_expander primitives_heap<TListTypes...>::complete_primitive( primitive_type primitive )
    {
      return vertex_expander( property_support, vertices, primitive );
    }

    // template< typename  ... TListTypes >
    // inline primitives_heap<TListTypes...>::index_expander primitives_heap<TListTypes...>::complete_indexed_primitive( primitive_type primitive )
    // {
    //   return index_expander( *this, primitive );
    // }

    // ---- ---- ---- ----






  }
}
