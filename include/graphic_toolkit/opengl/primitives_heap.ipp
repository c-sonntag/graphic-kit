#pragma once

#include <graphic_toolkit/opengl/primitives_heap.h>

#include <graphic_toolkit/opengl/vertex_expander.h>
#include <graphic_toolkit/opengl/index_expander.h>

#include <iostream>

namespace graphic_toolkit {
  namespace opengl {

    template< typename  ... TListTypes >
    template< typename... Args >
    inline primitives_heap<TListTypes...>::primitives_heap( Args ... attributes ) :
      attrib_pointers( attributes... ),
      initialized( false ),  busy( false ),
      property_support( *this ),
      conditionals_uniforms_sets( property_support.conditionals_uniforms_sets )
    { }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::init_buffer()
    {
      //
      if ( initialized )
        return;

      //
      check_not_busy();

      //
      if ( !vertices.rows.empty() )
        vertices.allocate();

      if ( !indices.indices.empty() )
        indices.allocate();

      //
      initialized = true;
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_buffer()
    {
      initialized = false;
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_expanders()
    {
      //
      check_not_busy();

      //
      expanders_properties.clear();
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_all()
    {
      reset_buffer();
      reset_expanders();
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::gl_attrib_pointer( QOpenGLFunctions_3_3_Core & gl )
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

    // ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program )
    {
      if ( !initialized )
        return;

      //
      if ( vertices.buffer.isCreated() )
        vertices.buffer.bind();
      if ( indices.buffer.isCreated() )
        indices.buffer.bind();

      //
      gl_attrib_pointer( gl );

      //
      for ( const abstract_expander_property_up_t & aep_up : expanders_properties )
        aep_up->draw( property_support, gl, program );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::auto_draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program )
    {
      attrib_array_enable_all( gl );
      draw( gl, program );
      attrib_array_disable_all( gl );
    }

    // ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::attrib_array_enable_all( QOpenGLFunctions_3_3_Core & gl )
    {
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.glEnableVertexAttribArray( apo.gl_location );
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::attrib_array_disable_all( QOpenGLFunctions_3_3_Core & gl )
    {
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.glDisableVertexAttribArray( apo.gl_location );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline primitives_heap<TListTypes...>::expander_property_support_inherited::expander_property_support_inherited( primitives_heap_t & _heap ) :
      heap( _heap )
    {}

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::expander_property_support_inherited::lock()
    {
      if ( heap.busy )
        throw std::runtime_error( "primitives_heap is already locked" );
      heap.busy = true;
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::expander_property_support_inherited::unlock( abstract_expander_property_up_t property_up )
    {
      if ( !heap.busy )
        throw std::runtime_error( "primitives_heap is not locked, or you want to unlock it" );
      if ( !property_up )
        throw std::runtime_error( "primitives_heap unlocker, need a valid 'abstract_expander_property' unque_ptr" );
      //
      heap.expanders_properties.emplace_back( std::move( property_up ) );
      heap.busy = false;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::check_not_busy() const
    {
      if ( busy )
        throw std::runtime_error( "primitives_heap is currently busy" );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::is_init() const
    {
      return initialized;
    }

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::is_busy() const
    {
      return busy;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::vertex_expander primitives_heap<TListTypes...>::complete_primitive( primitive_type primitive )
    {
      return vertex_expander( property_support, vertices, std::move(primitive) );
    }

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::index_expander primitives_heap<TListTypes...>::complete_indexed_primitive( primitive_type primitive )
    {
      return index_expander( property_support, vertices, indices, std::move(primitive) );
    }

    // ---- ---- ---- ----




  }
}
