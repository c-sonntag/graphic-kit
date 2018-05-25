#pragma once

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <stdexcept>


namespace graphic_toolkit {
  namespace opengl {

    struct core_context
    {
     private:
      static inline QOpenGLContext & get_context() {
        QOpenGLContext * const context( QOpenGLContext::currentContext() );
        if ( !context )
          throw std::runtime_error( "[core_context] can't get context" );
        return *context;
      }
      static inline QOpenGLFunctions_3_3_Core & get_core( QOpenGLContext & context ) {
        QOpenGLFunctions_3_3_Core * const core( context.versionFunctions<QOpenGLFunctions_3_3_Core>() );
        if ( !core )
          throw std::runtime_error( "[core_context] can't get opengl 330 core" );
        return *core;
      }

     public:
      QOpenGLContext & context;
      QOpenGLFunctions_3_3_Core & gl;

     public:
      inline core_context() :
        context( get_context() ), gl( get_core( context ) )
      {}
    };

  }
}


