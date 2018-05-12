#pragma once
#ifndef graphic_toolkit_opengl_index_expander_h
#define graphic_toolkit_opengl_index_expander_h

#include <graphic_toolkit/opengl/index_buffer.h>

#include <graphic_toolkit/opengl/index_buffer.h>

#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    template<typename  ... TListTypes>
    class vertex_expander;

    // ---- ---- ---- ----

    struct index_expander
    {
     public:
      using index_buffer_t = index_buffer;

     public:
      const primitive_type primitive;
      const size_t start;

     protected:
      size_t count;

     protected:
      index_expander( index_buffer_t & buffer );

     public:
      ~index_expander();

     public:
      void push( index_buffer_t::indice_t index );

     public:
      void reserve( size_t n );
    };

  }
}


#include <graphic_toolkit/opengl/index_expander.ipp>
#endif
