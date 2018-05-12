#pragma once

#include <graphic_toolkit/opengl/vertex_expander.h>

#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/index_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::vertex_expander( primitives_heap_t & _primitives_heap, primitive_type _primitive ) :
      primitives_heap( _primitives_heap ),
      primitive( std::move( _primitive ) ),
      start( primitives_heap.vertices.rows.size() )
    {
      primitives_heap.lock();
    }

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::~vertex_expander()
    {
      count = primitives_heap.vertices.rows.size() - start;
      primitives_heap.unlock();
    }

    template<typename  ... TListTypes>
    template< class... Args >
    inline void vertex_expander<TListTypes...>::push( Args && ... args )
    {
      primitives_heap.vertices.rows.push( args... );
    }

    template<typename  ... TListTypes>
    inline void vertex_expander<TListTypes...>::reserve( size_t n )
    {
      primitives_heap.vertices.rows.reserve( start + n );
    }

    template<typename  ... TListTypes>
    inline index_expander & vertex_expander<TListTypes...>::expand_to_index()
    {
      //
      if ( index_expander_up )
        throw std::runtime_error( "This vertex_expander already contains one index_expander_up" );

      //
      index_expander_up = std::make_unique<index_expander>( *this );

      //
      return *index_expander_up;
    }


  }
}

