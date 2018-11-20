#pragma once

#include <raiigl/gl_types.hpp>

#include <gtk/opengl/primitives_heap/vertex_buffer.hpp>
#include <gtk/opengl/primitives_heap/abstract_expander.hpp>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace gtk {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    // ---- ---- ---- ----

    struct vertex_expander_property : public abstract_expander_property
    {
     public:
      vertex_expander_property( raiigl::primitive_type _primitive, uint _start );
      virtual ~vertex_expander_property() = default;

     public:
      const uint start;

     public:
      uint count;

     protected:
      void gl_draw( const abstract_expander_property_support & bd, raiigl::gl330 & gl, raiigl::program & program ) const;
      void gl_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program ) const;

     protected:
      virtual void gl_multiple_instance_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program, const uint nb_instance ) const;
    };

    // ---- ----

    template<typename  ... TListTypes>
    struct vertex_expander : public abstract_expander<vertex_expander_property>
    {
     public:
      using vertex_expander_property_up_t = std::unique_ptr<vertex_expander_property>;
      using vertex_buffer_t = vertex_buffer<TListTypes...>;

     protected:
      vertex_buffer_t & vertices;

     protected:
      friend primitives_heap<TListTypes...>;
      vertex_expander( abstract_expander_property_support & _expander_support, vertex_buffer_t & _vertices, raiigl::primitive_type _primitive );

     public:
      // Enable move.
      vertex_expander( vertex_expander && ) = default;
      // Disable copy from lvalue.
      vertex_expander( const vertex_expander & ) = delete;
      vertex_expander & operator=( const vertex_expander & ) = delete;

     public:
      ~vertex_expander();

     public:
      template< class... Args >
      void push( Args && ... args );

      template< class... Args >
      void push( const Args && ... args );

      template< class... Args >
      void push( const Args & ... args );

      /**< @todo implement iterator push */
      //template< class... Args >
      //void push( const Args & ... args );

     public:
      void reserve( const size_t n );

    };

  }
}


#include <gtk/opengl/primitives_heap/vertex_expander.ipp>

