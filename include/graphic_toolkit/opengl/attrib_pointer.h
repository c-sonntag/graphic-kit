#pragma once
#ifndef graphic_toolkit_opengl_attrib_pointer_hpp
#define graphic_toolkit_opengl_attrib_pointer_hpp

#include <GL/gl.h>
#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    struct attrib_pointer
    {
     public:
      GLuint gl_location;
      GLint gl_tuple_size;
      GLenum gl_type;
      GLboolean gl_normalized;

     public:
      attrib_pointer(
        GLuint _location,
        GLint _tuple_size,
        GLenum _type,
        GLboolean _normalized
      );

    };

  }
}

#include <graphic_toolkit/opengl/attrib_pointer.ipp>
#endif
