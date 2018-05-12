#pragma once

#include <QOpenGLFunctions_3_3_Core>

namespace graphic_toolkit {
  namespace opengl {

    enum class primitive_type : GLint
    {
      points = GL_TRIANGLES,
      lines = GL_LINES, line_loop = GL_LINE_LOOP, line_strip = GL_LINE_STRIP,
      triangles = GL_TRIANGLES, triangle_strip = GL_TRIANGLE_STRIP, triangle_fan = GL_TRIANGLE_FAN,
    };

  }
}
