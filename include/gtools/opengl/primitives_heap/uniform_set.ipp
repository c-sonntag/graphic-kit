#pragma once

#include <gtools/opengl/primitives_heap/uniform_set.hpp>

namespace gtools {
  namespace opengl {

    template< class... Args >
    inline uniform_set::uniform_set( std::string _name, const Args & ...  args ) :
      name( std::move( _name ) ),
      program_setter( [ = ]( const raiigl::program & program ) { program.set_uniform_value( name, args... ); } )
    {}

    inline void uniform_set::set( const raiigl::program & program ) const
    {
      program_setter( program );
    }

    // ---- ---- ---- ----

    inline void conditional_uniform_set::enable_set( const std::string & condition_name )
    {
      conditional_activation[condition_name] = true;
    }
    inline void conditional_uniform_set::disable_set( const std::string & condition_name )
    {
      conditional_activation[condition_name] = false;
    }
    inline bool conditional_uniform_set::is_set( const std::string & condition_name ) const
    {
      const auto usca_pair_it = conditional_activation.find( condition_name );
      return ( usca_pair_it != conditional_activation.end() ) ?
             usca_pair_it->second : false;
    }


  }
}

