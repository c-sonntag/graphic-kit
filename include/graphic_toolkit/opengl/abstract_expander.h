#pragma once
#ifndef graphic_toolkit_opengl_abstract_expander_h
#define graphic_toolkit_opengl_abstract_expander_h

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/uniform_set.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace graphic_toolkit {
  namespace opengl {

    struct abstract_expander_property;

    // ---- ----

    struct expander_property_support
    {
     public:
      using abstract_expander_property_up_t = std::unique_ptr<abstract_expander_property>;

     public:
      virtual ~expander_property_support() = default;

     public:
      virtual void lock() = 0;
      virtual void unlock( abstract_expander_property_up_t property_up ) = 0;

     public:
      conditional_uniform_set conditional_uniforms_sets;
    };

    // ---- ----

    struct abstract_expander_property
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
      abstract_expander_property( primitive_type _primitive, size_t _start );
      virtual ~abstract_expander_property() = default;

     protected:
      void apply_uniform_sets( const expander_property_support & bd, QOpenGLShaderProgram & program ) const;

     public:
      void draw( const expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const;

     protected:
      virtual void gl_draw( const expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const = 0;

     public:
      template< class... Args >
      void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );
    };

    // ---- ----

    template<typename  ... TListTypes>
    class primitives_heap;

    struct abstract_expander
    {
     protected:
      expander_property_support & expander_support;

     public:
      abstract_expander( expander_property_support & _expander_support );

     public:

      // Enable move.
      abstract_expander( abstract_expander && ) = default;
      // Disable copy from lvalue.
      abstract_expander( const abstract_expander & ) = delete;
      abstract_expander & operator=( const abstract_expander & ) = delete;

    };


  }
}


#include <graphic_toolkit/opengl/abstract_expander.ipp>
#endif
