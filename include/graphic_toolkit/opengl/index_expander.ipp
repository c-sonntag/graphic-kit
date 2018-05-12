#pragma once

#include <graphic_toolkit/opengl/index_expander.h>

#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/vertex_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    inline vertex_expander::property::property( primitive_type _primitive, size_t _start ) :
      primitive( std::move( _primitive ) ),
      start( td::move( _start ) )
    { }

    inline size_t vertex_expander::property::get_count() const
    {
      return m_count;
    }

    // ---- ---- ---- ----



    inline index_expander::index_expander( index_buffer_t & _indices ) :
      start( primitives_heap.vertices.rows.size() )
    {
      primitives_heap.lock();
    }

    template<typename  ... TListTypes>
    inline index_expander::~index_expander()
    {
      count = primitives_heap.vertices.rows.size() - start;
      primitives_heap.unlock();
    }

    // ---- ---- ---- ----


    template<typename  ... TListTypes>
    template< class... Args >
    inline void index_expander::push( Args && ... args )
    {
      primitives_heap.vertices.rows.push( args... );
    }

    template<typename  ... TListTypes>
    inline void index_expander::reserve( size_t n )
    {
      primitives_heap.vertices.rows.reserve( start + n );
    }

    // ---- ---- ---- ----


  }
}

