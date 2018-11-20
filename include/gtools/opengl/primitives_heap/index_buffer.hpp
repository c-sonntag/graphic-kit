#pragma once

#include <raiigl/gl_types.hpp>
#include <raiigl/buffer.hpp>

#include <vector>

namespace gtools {
  namespace opengl {

    struct index_buffer
    {
     public:
      using indice_t = GLuint;
      using indices_t = std::vector<indice_t>;

     public:
      static constexpr size_t indice_size = sizeof( indice_t );
      static constexpr raiigl::indice_type indice_type = raiigl::indice_type::Uint;

     public:
      raiigl::buffer buffer;
      indices_t indices;

     public:
      index_buffer();

     protected:
      size_t buffer_length, buffer_bytesize;

     public:
      void allocate();

    };

  }
}

#include <gtools/opengl/primitives_heap/index_buffer.ipp>