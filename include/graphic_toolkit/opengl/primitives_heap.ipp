#pragma once

#include <graphic_toolkit/opengl/primitives_heap.h>

#include <graphic_toolkit/opengl/vertex_expander.h>

namespace graphic_toolkit {
  namespace opengl {

    template< typename  ... TListTypes >
    template< typename... Args >
    inline primitives_heap<TListTypes...>::primitives_heap( Args ... attributes ) :
      is_busy( false ),
      attrib_pointers( attributes... )
    { }

    // ---- ---- ---- ----

    template< typename  ... TListTypes >
    inline typename primitives_heap<TListTypes...>::expander primitives_heap<TListTypes...>::complete_primitive( primitive_type primitive )
    {
      return *expanders.emplace( expanders.end(),  *this, primitive );
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
    inline void primitives_heap<TListTypes...>::unlock()
    {
      is_busy = false;
    }




  }
}
