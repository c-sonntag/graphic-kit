#pragma once
#ifndef graphic_toolkit_opengl_primitives_heap_h
#define graphic_toolkit_opengl_primitives_heap_h

#include <graphic_toolkit/types.h>

#include <graphic_toolkit/opengl/primitive_type.h>

#include <graphic_toolkit/opengl/attrib_pointer.h>
#include <graphic_toolkit/opengl/attrib_pointer_by_offset.h>

#include <graphic_toolkit/opengl/vertex_buffer.h>
#include <graphic_toolkit/opengl/index_buffer.h>

#include <graphic_toolkit/opengl/abtract_expander_property.h>

#include <vector>
#include <array>
#include <list>

namespace graphic_toolkit {

  void test();

  namespace opengl {

    template<typename  ... TListTypes>
    struct vertex_expander;

    template<typename  ... TListTypes>
    struct index_expander;

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    class primitives_heap
    {
     public:
      static constexpr uint nb_types { sizeof...( TListTypes ) };

     protected:
      opengl::vertex_buffer<TListTypes...> vertices;
      opengl::index_buffer indices;

     protected:
      using attrib_pointers_t = attribs_pointers_by_offset<TListTypes...>;
      const attrib_pointers_t attrib_pointers;

     public:
      template< typename... Args >
      primitives_heap( Args ... attributes );

     protected:
      bool initialized;

     public:
      void init_buffer();
      void reset_buffer();
      void reset_expanders();
      void destroy_all();

     public:
      void draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program );

     public:
      void auto_draw( QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program );
      void attrib_array_enable_all( QOpenGLFunctions_3_3_Core & gl );
      void attrib_array_disable_all( QOpenGLFunctions_3_3_Core & gl );

     public:
      using expander_property_up_t = std::unique_ptr<opengl::abtract_expander_property>;
      using vertex_expander = opengl::vertex_expander<TListTypes...>;
      using index_expander = opengl::index_expander<TListTypes...>;

     private:
      bool is_busy;
      std::list<expander_property_up_t> expanders_properties;

     protected:
      //expander_property_support property_support;
      // friend vertex_expander;
      // friend index_expander;
      struct expander_property_support : public opengl::expander_property_support
      {
       protected:
        primitives_heap & heap;
       public:
        void lock();
        void unlock( expander_property_up_t property_up );
      } property_support;

     public:
      vertex_expander complete_primitive( primitive_type primitive );
      index_expander complete_indexed_primitive( primitive_type primitive );

    };

  }
}


#include <graphic_toolkit/opengl/primitives_heap.ipp>
#endif
