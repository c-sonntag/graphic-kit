#pragma once
#ifndef graphic_toolkit_opengl_index_expander_h
#define graphic_toolkit_opengl_index_expander_h

#include <graphic_toolkit/opengl/index_buffer.h>

#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    struct vertex_expander;

    // ---- ---- ---- ----

    struct index_expander
    {
     public:
      using index_buffer_t = index_buffer;

     public:
      struct property
      {
       public:
        const size_t start;

       public:
        size_t get_count() const;

       protected:
        size_t m_count;
      };

     public:
      using property_up_t = std::unique_ptr<property>;

     protected:
      index_expander( index_buffer_t & _indices );

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
