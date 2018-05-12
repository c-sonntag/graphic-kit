#pragma once

#include <graphic_toolkit/opengl/attrib_pointer.h>

namespace graphic_toolkit {
  namespace opengl {

    inline attrib_pointer::attrib_pointer(
      GLuint _location,
      GLint _tuple_size,
      GLenum _type,
      GLboolean _normalized
    ) :
      gl_location( std::move( _location ) ),
      gl_tuple_size( std::move( _tuple_size ) ),
      gl_type( std::move( _type ) ),
      gl_normalized( std::move( _normalized ) )
    {}

  }
}

