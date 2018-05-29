#pragma once
#ifndef graphic_toolkit_opengl_abstract_expander_h
#define graphic_toolkit_opengl_abstract_expander_h

#include <graphic_toolkit/opengl/abstract_expander_property_support.h>

#include <graphic_toolkit/opengl/primitive_type.h>
#include <graphic_toolkit/opengl/uniform_container.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace graphic_toolkit {
  namespace opengl {

    struct abstract_expander_property
    {
     public:
      uniform_container uniforms;

     public:
      const primitive_type primitive;

     public:
      abstract_expander_property( primitive_type _primitive );
      virtual ~abstract_expander_property() = default;

     public:
      void draw( const abstract_expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const;

     protected:
      virtual void gl_draw( const abstract_expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const = 0;
    };

    // ---- ----

    template<typename  ... TListTypes>
    class primitives_heap;

    template<class TExpanderProperty>
    struct abstract_expander
    {
     public:
      using expander_property_up_t = std::unique_ptr<TExpanderProperty>;

     protected:
      abstract_expander_property_support & expander_support;
      expander_property_up_t expander_property_up;

     public:
      abstract_expander( abstract_expander_property_support & _expander_support, expander_property_up_t _expander_property_up );
      virtual ~abstract_expander();

     public:

      // Enable move.
      abstract_expander( abstract_expander && ) = default;
      // Disable copy from lvalue.
      abstract_expander( const abstract_expander & ) = delete;
      abstract_expander & operator=( const abstract_expander & ) = delete;

     public:

      template< class... Args >
      void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );

    };


  }
}


#include <graphic_toolkit/opengl/abstract_expander.ipp>
#endif
