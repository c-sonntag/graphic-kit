#pragma once

#include <graphic_toolkit/opengl/abstract_expander.h>


namespace graphic_toolkit {
  namespace opengl {

    inline abstract_expander_property::abstract_expander_property( primitive_type _primitive, size_t _start ) :
      primitive( std::move( _primitive ) ),
      start( std::move( _start ) ),
      count( 0 )
    { }

    // ---- ----

    inline void abstract_expander_property::apply_uniform_sets( const expander_property_support & bd, QOpenGLShaderProgram & program ) const
    {
      if ( uniform_sets_up )
        for ( const uniform_set & us : *uniform_sets_up )
          us.set( program );
      if ( uniform_sets_conditional_up )
        for ( const typename uniform_sets_conditional_t::value_type & usc_pair : *uniform_sets_conditional_up )
          if ( bd.conditionals_uniforms_sets.is_set( usc_pair.first ) )
            for ( const uniform_set & usc_us : usc_pair.second )
              usc_us.set( program );
    }

    inline void abstract_expander_property::draw( const expander_property_support & bd, QOpenGLFunctions_3_3_Core & gl, QOpenGLShaderProgram & program ) const
    {
      //
      apply_uniform_sets( bd, program );

      //
      gl_draw( bd, gl, program );
    }

    // ---- ----

    template< class... Args >
    inline void abstract_expander_property::set_uniform( const std::string & var_name, Args... values )
    {
      if ( !uniform_sets_up )
      {
        uniform_sets_t * us_p = new uniform_sets_t();
        uniform_sets_up = uniform_sets_up_t( us_p );
      }
      uniform_sets_up->emplace_back( var_name, values... );
    }

    template< class... Args >
    inline void abstract_expander_property::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values )
    {
      if ( !uniform_sets_conditional_up )
      {
        uniform_sets_conditional_t * usc_p = new uniform_sets_conditional_t();
        uniform_sets_conditional_up = uniform_sets_conditional_up_t( usc_p );
      }

      //
      auto usc_pair_it = uniform_sets_conditional_up->find( condition_name );
      if ( usc_pair_it == uniform_sets_conditional_up->end() )
      {
        auto usc_emplacepair = uniform_sets_conditional_up->emplace( condition_name, uniform_sets_t() );
        usc_pair_it = usc_emplacepair.first;
      }

      //
      usc_pair_it->second.emplace_back( var_name, values... );
    }

    // ---- ---- ---- ----

    template<class TExpanderProperty>
    inline abstract_expander<TExpanderProperty>::abstract_expander( expander_property_support & _expander_support, expander_property_up_t _expander_property ) :
      expander_support( _expander_support ),
      expander_property( std::move(_expander_property) )
    {}

    // ---- ----

    template<class TExpanderProperty>
    template< class... Args >
    inline void abstract_expander<TExpanderProperty>::set_uniform( const std::string & var_name, Args... values )
    {
      expander_property->set_uniform( var_name, values... );
    }

    template<class TExpanderProperty>
    template< class... Args >
    inline void abstract_expander<TExpanderProperty>::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values )
    {
      expander_property->set_uniform_on_condition( condition_name, var_name, values... );
    }


  }
}

