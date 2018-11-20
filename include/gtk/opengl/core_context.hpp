#pragma once

#include <raiigl/gl_context.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/gl430.hpp>

#include <stdexcept>

namespace gtk {
  namespace opengl {

    struct core_context
    {
     private:
      static inline raiigl::gl_context & get_context() {
        raiigl::gl_context * const context( raiigl::gl_context::get_current() );
        if ( !context )
          throw std::runtime_error( "[core_context] can't get context" );
        return *context;
      }
      static inline raiigl::gl330 & get_core( raiigl::gl_context & context ) {
        raiigl::gl330 * const core( context.get_version<raiigl::gl330>() );
        if ( !core )
          throw std::runtime_error( "[core_context] can't get opengl 330" );
        return *core;
      }

     public:
      raiigl::gl_context & context;
      raiigl::gl330 & gl;

     public:
      inline core_context() :
        context( get_context() ), gl( get_core( context ) )
      {}
    };

  }
}


