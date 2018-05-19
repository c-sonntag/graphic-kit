#pragma once

#include <graphic_toolkit/opengl/quick_program.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    inline quick_program::quick_program( QString _vertex_path, QString _fragment_path, bool build_now ) :
      vertex_path( std::move( _vertex_path ) ),
      fragment_path( std::move( _fragment_path ) )
    {
      if ( build_now )
        build();
    }

    // ---- ----

    inline void quick_program::build()
    {
      if ( program.isLinked() )
        return;

      // Compile vertex shader
      if ( !program.addShaderFromSourceFile( QOpenGLShader::Vertex, vertex_path ) )
        throw std::runtime_error( ( "Compile vertex shader : " + vertex_path ).toStdString() );

      // Compile fragment shader
      if ( !program.addShaderFromSourceFile( QOpenGLShader::Fragment, fragment_path ) )
        throw std::runtime_error( ( "Compile fragment shader :" + fragment_path ).toStdString() );

      // Link shader pipeline
      if ( !program.link() )
        throw std::runtime_error( "Link shader pipeline" );
    }

  }
}

