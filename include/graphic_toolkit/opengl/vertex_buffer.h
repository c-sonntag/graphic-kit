#pragma once
#ifndef graphic_toolkit_opengl_vertex_buffer_h
#define graphic_toolkit_opengl_vertex_buffer_h

#include <raiigl/buffer.hpp>

#include <vector>
#include <tuple>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    struct vertex_buffer
    {
     public:
      using row_t = std::tuple<TListTypes...>;
      using rows_t = std::vector<row_t>;

     public:
      static constexpr size_t row_size = sizeof( row_t );

      public:
      raiigl::buffer buffer;
      rows_t rows;

     public:
      vertex_buffer();

    // protected:
      size_t buffer_length, buffer_bytesize;

     public:
      void allocate();
    };

  }
}


#include <graphic_toolkit/opengl/vertex_buffer.ipp>
#endif
