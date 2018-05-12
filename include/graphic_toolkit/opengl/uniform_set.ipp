#pragma once

#include <graphic_toolkit/opengl/uniform_set.h>

namespace graphic_toolkit {
  namespace opengl {

    template< class... Args >
    inline uniform_set::uniform_set( std::string _name, Args && ...  args ) :
      name( std::move( _name ) ),
      program_setter( [ = ]( QOpenGLShaderProgram & program ) { program.setUniformValue( name.c_str(), args... ); } )
    {}

    inline void uniform_set::set( QOpenGLShaderProgram & program ) const
    {
      program_setter( program );
    }

  }
}

