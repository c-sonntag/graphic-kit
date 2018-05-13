#include <graphic_toolkit/__opengl_drawer.h>

namespace graphic_toolkit {

  std::unique_ptr<QOpenGLShaderProgram> opengl_quick_program( const QString & vertex_path, const QString & fragment_path )
  {
    std::unique_ptr<QOpenGLShaderProgram> program_up( new QOpenGLShaderProgram() );

    // Compile vertex shader
    if ( !program_up->addShaderFromSourceFile( QOpenGLShader::Vertex, vertex_path ) )
      throw std::runtime_error( "Compile vertex shader" );

    // Compile fragment shader
    if ( !program_up->addShaderFromSourceFile( QOpenGLShader::Fragment, fragment_path ) )
      throw std::runtime_error( "Compile fragment shader" );

    // Link shader pipeline
    if ( !program_up->link() )
      throw std::runtime_error( "Link shader pipeline" );

    //
    return program_up;
  }

  // ---- ---- ---- ----

}
