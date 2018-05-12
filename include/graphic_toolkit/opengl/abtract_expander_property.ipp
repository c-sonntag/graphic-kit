#pragma once

#include <graphic_toolkit/opengl/abtract_expander_property.h>


namespace graphic_toolkit {
  namespace opengl {

    inline abtract_expander_property::abtract_expander_property( primitive_type _primitive, size_t _start ) :
      primitive( std::move( _primitive ) ),
      start( std::move( _start ) ),
      count( 0 )
    { }


    // ---- ----

    template< class... Args >
    inline void abtract_expander_property::set_uniform( const std::string & var_name, Args... values )
    {
      if ( !uniform_sets_up )
      {
        uniform_sets_t * us_p = new uniform_sets_t();
        uniform_sets_up = uniform_sets_up_t( us_p );
      }
      uniform_sets_up->emplace_back( var_name, values... );
    }

    template< class... Args >
    inline void abtract_expander_property::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values )
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

