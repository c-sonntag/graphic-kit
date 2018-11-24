#pragma once

#include <gk/opengl/primitives_heap/abstract_expander.hpp>

#include <gk/opengl/primitives_heap/uniform_container.hpp>

namespace gk {
  namespace opengl {


    inline abstract_expander_property::abstract_expander_property( raiigl::primitive_type _primitive ) :
      primitive( std::move( _primitive ) )
    { }

    // ---- ----

    inline void abstract_expander_property::draw( const abstract_expander_property_support & bd, raiigl::gl330 & gl, raiigl::program & program ) const
    {
      //
      uniforms.apply_uniform_sets( bd, program );

      //
      gl_draw( bd, gl, program );
    }

    inline void abstract_expander_property::multiple_instance_draw( const abstract_expander_property_support & bd, raiigl::gl430 & gl, raiigl::program & program, const uint nb_instance ) const
    {
      //
      uniforms.apply_uniform_sets( bd, program );

      //
      gl_multiple_instance_draw( bd, gl, program, nb_instance );
    }

    // ---- ---- ---- ----

    template<class TExpanderProperty>
    inline abstract_expander<TExpanderProperty>::abstract_expander( abstract_expander_property_support & _expander_support, expander_property_up_t _expander_property_up ) :
      expander_support( _expander_support ),
      expander_property_up( std::move( _expander_property_up ) )
    { expander_support.lock(); }

    template<class TExpanderProperty>
    inline abstract_expander<TExpanderProperty>::~abstract_expander()
    { expander_support.unlock( std::move( expander_property_up ) ); }

    // ---- ----

    template<class TExpanderProperty>
    template< class... Args >
    __forceinline void abstract_expander<TExpanderProperty>::set_uniform( const std::string & var_name, const Args & ... values )
    { expander_property_up->uniforms.set_uniform( var_name, values... ); }

   // template<class TExpanderProperty>
   // template< class... Args >
   // __forceinline void abstract_expander<TExpanderProperty>::set_uniform( const GLint id, const Args & ... values )
   // { expander_property_up->uniforms.set_uniform( id, values... ); }

   // template<class TExpanderProperty>
   // template< class... Args >
   // __forceinline void abstract_expander<TExpanderProperty>::set_uniform( const raiigl::uniform_variable & uv, const Args & ... values )
   // { set_uniform( uv.id, values... ); }

    // ---- ----

    template<class TExpanderProperty>
    template< class... Args >
    __forceinline void abstract_expander<TExpanderProperty>::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, const Args & ... values )
    { expander_property_up->uniforms.set_uniform_on_condition( condition_name, var_name, values... ); }

   // template<class TExpanderProperty>
   // template< class... Args >
   // __forceinline void abstract_expander<TExpanderProperty>::set_uniform_on_condition( const std::string & condition_name, const GLint id, const Args & ... values )
   // { expander_property_up->uniforms.set_uniform_on_condition( condition_name, id, values... ); }

   // template<class TExpanderProperty>
   // template< class... Args >
   // __forceinline void abstract_expander<TExpanderProperty>::set_uniform_on_condition( const std::string & condition_name, const raiigl::uniform_variable & uv, const Args & ... values )
   // { set_uniform_on_condition( condition_name, uv.id, values... ); }


  }
}

