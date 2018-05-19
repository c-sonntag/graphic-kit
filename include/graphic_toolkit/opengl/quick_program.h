#pragma once
#ifndef graphic_toolkit_opengl_quick_program_h
#define graphic_toolkit_opengl_quick_program_h


#include <memory>

#include <QString>
#include <QOpenGLShaderProgram>

namespace graphic_toolkit {
  namespace opengl {

    struct quick_program
    {
     public:
      const QString vertex_path;
      const QString fragment_path;

     public:
      QOpenGLShaderProgram program;

     public:
      quick_program( QString _vertex_path, QString _fragment_path, bool build_now = false );

     public:
      void build();

    };

  }
}

#include <graphic_toolkit/opengl/quick_program.ipp>
#endif

