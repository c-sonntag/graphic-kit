#pragma once

#include <graphic_toolkit/opengl/primitives_heap.h>

#include <graphic_toolkit/opengl/primitives_heap/vertex_expander.h>
#include <graphic_toolkit/opengl/primitives_heap/index_expander.h>
#include <graphic_toolkit/opengl/primitives_heap/uniform_lap.h>

#include <iostream>

namespace graphic_toolkit {
  namespace opengl {

    template< typename  ... TListTypes >
    template< typename... Args >
    inline primitives_heap<TListTypes...>::primitives_heap( Args ... attributes ) :
      attrib_pointers( attributes... ),
      initialized( false ), vao_is_configured( false ),
      busy( false ),
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
      vao_is_configured = false;
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_expanders()
    {
      //
      check_not_busy();

      //
      expanders_properties.clear();

      //
      vertices.rows.clear();
      indices.indices.clear();
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_uniforms_laps()
    {
      //
      check_not_busy();

      //
      uniforms_laps.clear();
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::reset_all()
    {
      reset_buffer();
      reset_expanders();
      reset_uniforms_laps();
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::gl_attrib_pointer( const raiigl::gl330 & gl )
    {
      /**< @see #way3 https://stackoverflow.com/a/39684775  */
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.vertex_attrib_pointer(
          apo.gl_location,
          apo.gl_tuple_size,
          apo.type,
          apo.gl_normalized,
          vertices_t::row_size,
          reinterpret_cast<const GLvoid *>( apo.opposed_offset ) /**< std::tuple have data inverted order */
        );
    }

    // ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::attrib_array_enable_all( const raiigl::gl330 & gl )
    {
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.enable_vertex_attrib_array( apo.gl_location );
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::attrib_array_disable_all( const raiigl::gl330 & gl )
    {
      for ( const attrib_pointer_by_offset & apo : attrib_pointers )
        gl.disable_vertex_attrib_array( apo.gl_location );
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::bind_buffer()
    {
      vertices.buffer.bind();
      indices.buffer.bind();
    }

    // ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::draw( const raiigl::gl330 & gl, raiigl::program & program )
    {
      if ( !initialized )
        return;

      //
      vao.bind();

      //
      if ( !vao_is_configured )
        attrib_array_enable_all( gl );

      //
      bind_buffer();

      //
      if ( !vao_is_configured )
        gl_attrib_pointer( gl );

      //
      if ( !vao_is_configured )
        vao_is_configured = true;

      //
      if ( have_uniforms_laps() )
        for ( const abstract_expander_property_support::uniform_container_up_t & uniforms_up : uniforms_laps )
        {
          if ( uniforms_up )
            uniforms_up->apply_uniform_sets( property_support, program );

          //
          for ( const abstract_expander_property_up_t & aep_up : expanders_properties )
            aep_up->draw( property_support, gl, program );
        }
      else
        for ( const abstract_expander_property_up_t & aep_up : expanders_properties )
          aep_up->draw( property_support, gl, program );

      //
      vao.unbind();

    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::expander_property_support_inherited::check_busy()
    {
      if ( !heap.busy )
        throw std::runtime_error( "primitives_heap is not locked, or you want to unlock it" );
    }

    // ---- ----

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
      check_busy();

      //
      if ( !property_up )
        throw std::runtime_error( "primitives_heap unlocker, need a valid 'abstract_expander_property' unique_ptr" );
      //
      if ( property_up->primitive != raiigl::primitive_type::None )
        heap.expanders_properties.emplace_back( std::move( property_up ) );

      //
      heap.busy = false;
    }

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::expander_property_support_inherited::unlock( uniform_container_up_t uniform_container_up )
    {
      check_busy();

      //
      if ( uniform_container_up )
        heap.uniforms_laps.emplace_back( std::move( uniform_container_up ) );

      //
      heap.busy = false;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline void primitives_heap<TListTypes...>::check_not_busy() const
    {
      if ( busy )
        throw std::runtime_error( "primitives_heap is currently busy" );
    }

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::empty() const
    {
      return expanders_properties.empty();
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::is_init() const
    {
      return initialized;
    }

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::have_uniforms_laps() const
    {
      return !uniforms_laps.empty();
    }

    template< typename  ... TListTypes >
    inline bool primitives_heap<TListTypes...>::is_busy() const
    {
      return busy;
    }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::vertex_expander primitives_heap<TListTypes...>::complete_primitive( raiigl::primitive_type primitive )
    {
      return vertex_expander( property_support, vertices, std::move( primitive ) );
    }

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::index_expander primitives_heap<TListTypes...>::complete_indexed_primitive( raiigl::primitive_type primitive )
    {
      return index_expander( property_support, vertices, indices, std::move( primitive ) );
    }

    // ---- ----

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::uniform_lap primitives_heap<TListTypes...>::complete_uniform_lap()
    {
      return uniform_lap( property_support );
    }

  }
}
