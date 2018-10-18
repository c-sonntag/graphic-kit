#pragma once

#include <graphic_toolkit/opengl/primitives_heap/vertex_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    inline vertex_expander_property::vertex_expander_property( raiigl::primitive_type _primitive, size_t _start ) :
      abstract_expander_property( std::move( _primitive ) ),
      start( std::move( _start ) ),
      count( 0 )
    { }

    inline void vertex_expander_property::gl_draw( const abstract_expander_property_support &, raiigl::gl330 & gl, raiigl::program & ) const
    {
      if ( count > 0 )
        gl.draw_arrays( primitive, start, count );
    }

    inline void vertex_expander_property::gl_draw( const abstract_expander_property_support &, raiigl::gl430 & gl, raiigl::program & ) const
    {
      if ( count > 0 )
        gl.draw_arrays( primitive, start, count );
    }


    // ---- ---- ---- ----

    inline void vertex_expander_property::gl_multiple_instance_draw( const abstract_expander_property_support & aeps, raiigl::gl430 & gl, raiigl::program & proc, const uint nb_instance ) const
    {
      gl.draw_arrays_instanced_base_instance(
        primitive, start, count,
        nb_instance, 0
      );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::vertex_expander( abstract_expander_property_support & _expander_support, vertex_buffer_t & _vertices, raiigl::primitive_type _primitive ) :
      abstract_expander(
        _expander_support,
        std::make_unique<vertex_expander_property>( std::move( _primitive ), _vertices.rows.size() )
      ),
      vertices( _vertices )
    { }

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::~vertex_expander()
    { expander_property_up->count = vertices.rows.size() - expander_property_up->start; }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void vertex_expander<TListTypes...>::push( Args && ... args )
    { vertices.rows.emplace_back( args... ); }

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void vertex_expander<TListTypes...>::push( const Args && ... args )
    { vertices.rows.emplace_back( args... ); }

    template<typename  ... TListTypes>
    template< class... Args >
    __forceinline void vertex_expander<TListTypes...>::push( const Args & ... args )
    { vertices.rows.emplace_back( args... ); }

    // ---- ----

    template<typename  ... TListTypes>
    __forceinline void vertex_expander<TListTypes...>::reserve( const size_t n )
    { vertices.rows.reserve( expander_property_up->start + n ); }

  }
}

