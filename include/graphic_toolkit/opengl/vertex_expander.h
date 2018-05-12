#pragma once
#ifndef graphic_toolkit_opengl_vertex_expander_h
#define graphic_toolkit_opengl_vertex_expander_h

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/vertex_buffer.h>
#include <graphic_toolkit/opengl/uniform_set.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

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

     public:
      struct property
      {
       public:
        using uniform_sets_t = std::list<uniform_set>;
        using uniform_sets_up_t = std::unique_ptr<uniform_sets_t>;
        using uniform_sets_conditional_t = std::unordered_map<std::string, uniform_sets_t>;
        using uniform_sets_conditional_up_t = std::unique_ptr<uniform_sets_conditional_t>;

       public:
        const primitive_type primitive;
        const size_t start;

      public:
        property(primitive_type _primitive, size_t _start);

       public:
        size_t get_count() const;

       protected:
        size_t m_count;
      };

     public:
      using property_up_t = std::unique_ptr<property>;

     protected:
      primitives_heap_t & primitives_heap;
      vertex_buffer_t & vertices;

     protected:
      property_up_t property_up;

     protected:
      vertex_expander( primitives_heap_t & _primitives_heap, primitive_type _primitive );

     public:
      ~vertex_expander();

     public:
      template< class... Args >
      void push( Args && ... args );

     public:
      void reserve( size_t n );

     public:
      template< class... Args >
      inline void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      inline void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );

     protected:
      std::unique_ptr<index_expander> index_expander_up;

     public:
      index_expander & expand_to_index();

    };

  }
}


#include <graphic_toolkit/opengl/vertex_expander.ipp>
#endif
