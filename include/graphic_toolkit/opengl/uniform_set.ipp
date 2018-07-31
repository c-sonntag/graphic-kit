#pragma once

#include <graphic_toolkit/opengl/uniform_set.h>

namespace graphic_toolkit {
  namespace opengl {

    template< class... Args >
    inline uniform_set::uniform_set( std::string _name, Args && ...  args ) :
      name( std::move( _name ) ),
      program_setter( [ = ]( raiigl::program & program ) { program.setUniformValue( name.c_str(), args... ); } )
    {}

    inline void uniform_set::set( raiigl::program & program ) const
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

