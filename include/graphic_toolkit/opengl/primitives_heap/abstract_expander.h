#pragma once

#include <raiigl/gl_types.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/gl430.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <graphic_toolkit/opengl/primitives_heap/abstract_expander_property_support.h>
#include <graphic_toolkit/opengl/primitives_heap/uniform_container.h>

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
      const raiigl::primitive_type primitive;

     public:
      abstract_expander_property( raiigl::primitive_type _primitive );
      virtual ~abstract_expander_property() = default;

     public:
      void draw( const abstract_expander_property_support & bd, raiigl::gl330 & gl, raiigl::program & program ) const;

     protected:
      virtual void gl_draw( const abstract_expander_property_support & bd, raiigl::gl330 & gl, raiigl::program & program ) const = 0;
      virtual void gl_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program ) const = 0;
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
      void set_uniform( const std::string & var_name, const Args & ... values );

      // template< class... Args >
      // void set_uniform( const GLint id, const Args & ... values );
      //
      // template< class... Args >
      // void set_uniform( const raiigl::uniform_variable & uv, const Args & ... values );

     public:
      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, const Args & ... values );

      // template< class... Args >
      // void set_uniform_on_condition( const std::string & condition_name, const GLint id, const Args & ... values );
      //
      // template< class... Args >
      // void set_uniform_on_condition( const std::string & condition_name, const raiigl::uniform_variable & uv, const Args & ... values );

    };


  }
}


#include <graphic_toolkit/opengl/primitives_heap/abstract_expander.ipp>
