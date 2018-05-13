#pragma once
#ifndef graphic_toolkit_opengl_quick_program_h
#define graphic_toolkit_opengl_quick_program_h


#include <memory>

#include <QString>
#include <QOpenGLShaderProgram>

namespace graphic_toolkit {
  namespace opengl {

    std::unique_ptr<QOpenGLShaderProgram> quick_program( const QString & vertex_path, const QString & fragment_path );

  }
}

#include <graphic_toolkit/opengl/quick_program.ipp>
#endif

