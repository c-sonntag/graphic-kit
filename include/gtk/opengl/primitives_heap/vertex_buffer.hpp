#pragma once

#include <raiigl/buffer.hpp>

#include <vector>
#include <tuple>

namespace gtk {
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


#include <gtk/opengl/primitives_heap/vertex_buffer.ipp>
