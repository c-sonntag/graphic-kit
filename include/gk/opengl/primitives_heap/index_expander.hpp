#pragma once

#include <raiigl/gl_types.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/program.hpp>

#include <gk/opengl/primitives_heap/vertex_buffer.hpp>
#include <gk/opengl/primitives_heap/index_buffer.hpp>

#include <gk/opengl/primitives_heap/abstract_expander.hpp>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace gk {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    // ---- ---- ---- ----

    struct index_expander_property : public abstract_expander_property
    {
     public:
      index_expander_property( raiigl::primitive_type _primitive, size_t _vertex_start, size_t _index_start );
      virtual ~index_expander_property() = default;

     public:
      const size_t vertex_start, index_start;

     public:
      size_t vertex_count, index_count;

     public:
      bool indice_is_global = false;

     protected:
      void gl_draw( const abstract_expander_property_support & bd, raiigl::gl330 & gl, raiigl::program & program ) const;
      void gl_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program ) const;

    protected:
      virtual void gl_multiple_instance_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program, const uint nb_instance ) const;
    };

    // ---- ----

    template<typename  ... TListTypes>
    struct index_expander : public abstract_expander<index_expander_property>
    {
     public:
      using index_expander_property_up_t = std::unique_ptr<index_expander_property>;
      using vertex_buffer_t = vertex_buffer<TListTypes...>;
      using index_buffer_t = index_buffer;

     protected:
      vertex_buffer_t & vertices;
      index_buffer_t & indices;

     protected:
      const size_t vertex_start_decal;

     protected:
      friend primitives_heap<TListTypes...>;
      index_expander( abstract_expander_property_support & _expander_support, vertex_buffer_t & _vertices, index_buffer_t & _indices, raiigl::primitive_type _primitive );

     public:

      // Enable move.
      index_expander( index_expander && ) = default;
      // Disable copy from lvalue.
      index_expander( const index_expander & ) = delete;
      index_expander & operator=( const index_expander & ) = delete;

     public:
      ~index_expander();

     public:
      template< class... Args >
      void push_vertex( Args && ... args );

      template< class... Args >
      void push_vertex( const Args && ... args );

      template< class... Args >
      void push_vertex( const Args & ... args );

     public:
      void push_index( index_buffer_t::indice_t i );

     public:
      void set_global_indice( bool v );

     public:
      void reserve_vertices( size_t n );
      void reserve_indices( size_t n );

    };

  }
}


#include <gk/opengl/primitives_heap/index_expander.ipp>
