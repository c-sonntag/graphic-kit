#pragma once

#include <raiigl/gl_types.hpp>

#include <memory>

namespace gtools {
  namespace opengl {

    struct attrib_pointer
    {
     public:
      GLuint gl_location;
      GLint gl_tuple_size;
      raiigl::data_type type;
      bool gl_normalized;

     public:
      inline attrib_pointer(
        const GLuint _location,
        const GLint _tuple_size,
        const raiigl::data_type _type,
        const bool _normalized
      ) :
        gl_location( std::move( _location ) ),
        gl_tuple_size( std::move( _tuple_size ) ),
        type( std::move( _type ) ),
        gl_normalized( std::move( _normalized ) )
      {}

    };

  }
}
