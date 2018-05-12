#pragma once

#include <graphic_toolkit/opengl/index_expander.h>

#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/vertex_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    inline index_expander<TListTypes...>::index_expander( primitives_heap_t & _primitives_heap, primitive_type _primitive ) :
      primitives_heap( _primitives_heap ),
      primitive( std::move( _primitive ) ),
      start( primitives_heap.vertices.rows.size() )
    {
      primitives_heap.lock();
    }

    template<typename  ... TListTypes>
    inline index_expander<TListTypes...>::~index_expander()
    {
      count = primitives_heap.vertices.rows.size() - start;
      primitives_heap.unlock();
    }

    template<typename  ... TListTypes>
    template< class... Args >
    inline void index_expander<TListTypes...>::push( Args && ... args )
    {
      primitives_heap.vertices.rows.push( args... );
    }

    template<typename  ... TListTypes>
    inline void index_expander<TListTypes...>::reserve( size_t n )
    {
      primitives_heap.vertices.rows.reserve( start + n );
    }

  }
}

