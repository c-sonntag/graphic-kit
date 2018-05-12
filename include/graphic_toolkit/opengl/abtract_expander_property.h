#pragma once
#ifndef graphic_toolkit_opengl_abtract_expander_property_h
#define graphic_toolkit_opengl_abtract_expander_property_h

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/uniform_set.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace graphic_toolkit {
  namespace opengl {


    struct expander_property_support
    {
     public:
      using up_t = std::unique_ptr<expander_property_support>;

     public:
      virtual ~expander_property_support() = default;

     public:
      virtual void lock() = 0;
      virtual void unlock( up_t property_up ) = 0;

     public:
      conditional_uniform_set conditional_uniform_set;
    };


    struct abtract_expander_property
    {
     public:
      using uniform_sets_t = std::list<uniform_set>;
      using uniform_sets_up_t = std::unique_ptr<uniform_sets_t>;
      using uniform_sets_conditional_t = std::unordered_map<std::string, uniform_sets_t>;
      using uniform_sets_conditional_up_t = std::unique_ptr<uniform_sets_conditional_t>;

     public:
      const primitive_type primitive;
      const size_t start;

     public:
      size_t count;

     public:
      uniform_sets_up_t uniform_sets_up;
      uniform_sets_conditional_up_t uniform_sets_conditional_up;

     public:
      abtract_expander_property( primitive_type _primitive, size_t _start );
      virtual ~abtract_expander_property() = default;

     public:
      virtual void gl_draw( const expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const = 0;

     public:
      template< class... Args >
      void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );
    };




  }
}


#include <graphic_toolkit/opengl/abtract_expander_property.ipp>
#endif
