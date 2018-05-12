#pragma once
#ifndef graphic_toolkit_opengl_vertex_expander_h
#define graphic_toolkit_opengl_vertex_expander_h

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/vertex_buffer.h>

#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    struct index_expander;

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    struct vertex_expander
    {
     public:
      using primitives_heap_t = primitives_heap<TListTypes...>;
      using vertex_buffer_t = vertex_buffer<TListTypes...>;

     protected:
      friend primitives_heap_t;
      primitives_heap_t & primitives_heap;

     public:
      const primitive_type primitive;
      const size_t start;

     protected:
      size_t count;

     protected:
      vertex_expander( primitives_heap_t & drawer, primitive_type _primitive );

     public:
      ~vertex_expander();

     public:
      template< class... Args >
      void push( Args && ... args );

     public:
      void reserve( size_t n );

     protected:
      std::unique_ptr<index_expander> index_expander_up;

     public:
      index_expander & expand_to_index();

    };

  }
}


#include <graphic_toolkit/opengl/vertex_expander.ipp>
#endif
