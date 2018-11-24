#pragma once

#include <gk/opengl/primitives_heap/uniform_container.hpp>

namespace gk {
  namespace opengl {

    inline void uniform_container::apply_uniform_sets( const abstract_expander_property_support & bd, raiigl::program & program ) const
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

    template< class... Args >
    inline void uniform_container::set_uniform( const std::string & var_name, const Args & ... values )
    {
      if ( !uniform_sets_up )
      {
        uniform_sets_t * us_p = new uniform_sets_t();
        uniform_sets_up = uniform_sets_up_t( us_p );
      }
      uniform_sets_up->emplace_back( var_name, values... );
    }

    template< class... Args >
    inline void uniform_container::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, const Args & ... values )
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

  }
}

