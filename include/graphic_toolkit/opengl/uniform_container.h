#pragma once
#ifndef graphic_toolkit_opengl_uniform_container_h
#define graphic_toolkit_opengl_uniform_container_h

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/uniform_set.h>

#include <graphic_toolkit/opengl/abstract_expander_property_support.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace graphic_toolkit {
  namespace opengl {

    struct uniform_container
    {
     public:
      using uniform_sets_t = std::list<uniform_set>;
      using uniform_sets_up_t = std::unique_ptr<uniform_sets_t>;
      using uniform_sets_conditional_t = std::unordered_map<std::string, uniform_sets_t>;
      using uniform_sets_conditional_up_t = std::unique_ptr<uniform_sets_conditional_t>;

     public:
      uniform_sets_up_t uniform_sets_up;
      uniform_sets_conditional_up_t uniform_sets_conditional_up;

     public:
      void apply_uniform_sets( const abstract_expander_property_support & bd, QOpenGLShaderProgram & program ) const;

     public:
      template< class... Args >
      void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );
    };

  }
}


#include <graphic_toolkit/opengl/uniform_container.ipp>
#endif
