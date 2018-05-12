#pragma once
#ifndef graphic_toolkit_opengl_primitives_heap_h
#define graphic_toolkit_opengl_primitives_heap_h

#include <graphic_toolkit/types.h>

#include <graphic_toolkit/opengl/primitive_type.h>

#include <graphic_toolkit/opengl/attrib_pointer.h>
#include <graphic_toolkit/opengl/attrib_pointer_by_offset.h>

#include <graphic_toolkit/opengl/vertex_buffer.h>
#include <graphic_toolkit/opengl/index_buffer.h>

#include <vector>
#include <array>
#include <list>

namespace graphic_toolkit {

  void test();

  namespace opengl {

    template<typename  ... TListTypes>
    struct vertex_expander;

    template<typename  ... TListTypes>
    class primitives_heap
    {
     public:
      static constexpr uint nb_types { sizeof...( TListTypes ) };

     public:
      using expander = opengl::vertex_expander<TListTypes...>;

     protected:
      opengl::vertex_buffer<TListTypes...> vertices;
      opengl::index_buffer indices;

     protected:
      using attrib_pointers_t = attribs_pointers_by_offset<TListTypes...>;
      const attrib_pointers_t attrib_pointers;

     public:
      template< typename... Args >
      primitives_heap( Args ... attributes );

     private:
      bool is_busy;
      std::list<expander> expanders;

     protected:
      friend expander;
      void lock();
      void unlock();

     public:
      expander & complete_primitive( primitive_type primitive );

    };

  }
}


#include <graphic_toolkit/opengl/primitives_heap.ipp>
#endif
