#pragma once
#ifndef graphic_toolkit_opengl_index_expander_h
#define graphic_toolkit_opengl_index_expander_h

#include <graphic_toolkit/opengl/primitive_type.h>

#include <graphic_toolkit/opengl/vertex_buffer.h>
#include <graphic_toolkit/opengl/index_buffer.h>

#include <graphic_toolkit/opengl/abstract_expander.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    // ---- ---- ---- ----

    struct index_expander_property : public abstract_expander_property
    {
     public:
      index_expander_property( primitive_type _primitive, size_t _vertex_start, size_t _index_start );
      virtual ~index_expander_property() = default;

     public:
      const size_t vertex_start, index_start;

     public:
      size_t vertex_count, index_count;

     public:
      bool indice_is_global = false;

     protected:
      void gl_draw( const abstract_expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const;
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
      index_expander( abstract_expander_property_support & _expander_support, vertex_buffer_t & _vertices, index_buffer_t & _indices, primitive_type _primitive );

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

      void push_index( index_buffer_t::indice_t i );

     public:
      void set_global_indice( bool v );

     public:
      void reserve_vertices( size_t n );
      void reserve_indices( size_t n );

    };

  }
}


#include <graphic_toolkit/opengl/index_expander.ipp>
#endif
