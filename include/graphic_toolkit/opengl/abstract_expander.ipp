#pragma once

#include <graphic_toolkit/opengl/abstract_expander.h>

#include <graphic_toolkit/opengl/uniform_container.h>

namespace graphic_toolkit {
  namespace opengl {


    inline abstract_expander_property::abstract_expander_property( raiigl::primitive_type _primitive ) :
      primitive( std::move( _primitive ) )
    { }

    // ---- ----

    inline void abstract_expander_property::draw( const abstract_expander_property_support & bd, const raiigl::gl330 & gl, raiigl::program & program ) const
    {
      //
      uniforms.apply_uniform_sets( bd, program );

      //
      gl_draw( bd, gl, program );
    }

    // ---- ---- ---- ----

    template<class TExpanderProperty>
    inline abstract_expander<TExpanderProperty>::abstract_expander( abstract_expander_property_support & _expander_support, expander_property_up_t _expander_property_up ) :
      expander_support( _expander_support ),
      expander_property_up( std::move( _expander_property_up ) )
    {
      expander_support.lock();
    }

    template<class TExpanderProperty>
    inline abstract_expander<TExpanderProperty>::~abstract_expander()
    {
      expander_support.unlock( std::move( expander_property_up ) );
    }

    // ---- ----

    template<class TExpanderProperty>
    template< class... Args >
    inline void abstract_expander<TExpanderProperty>::set_uniform( const std::string & var_name, const Args & ... values )
    {
      expander_property_up->uniforms.set_uniform( var_name, values... );
    }

    template<class TExpanderProperty>
    template< class... Args >
    inline void abstract_expander<TExpanderProperty>::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, const Args & ... values )
    {
      expander_property_up->uniforms.set_uniform_on_condition( condition_name, var_name, values... );
    }


  }
}

