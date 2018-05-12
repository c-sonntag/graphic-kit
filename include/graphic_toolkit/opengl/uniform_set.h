#pragma once
#ifndef graphic_toolkit_opengl_uniform_set_hpp
#define graphic_toolkit_opengl_uniform_set_hpp

#include <string>
#include <QOpenGLShaderProgram>

namespace graphic_toolkit {
  namespace opengl {

    struct uniform_set
    {
     protected:
      const std::string name;
      const std::function<void( QOpenGLShaderProgram & )> program_setter;

     public:
      template< class... Args >
      inline uniform_set( std::string _name, Args && ...  args );

     public:
      void inline set( QOpenGLShaderProgram & program ) const;
    };

  }
}


#include <graphic_toolkit/opengl/uniform_set.ipp>
#endif
