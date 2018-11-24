#pragma once

#include <gk/opengl/primitives_heap/uniform_lap.hpp>

#include <gk/opengl/primitives_heap/uniform_container.hpp>

#include <stdexcept>

namespace gk {
  namespace opengl {

    inline uniform_lap::uniform_lap( abstract_expander_property_support & _expander_support ) :
      expander_support( _expander_support )
    {
      expander_support.lock();
    }

    inline uniform_lap::~uniform_lap()
    {
      expander_support.unlock( std::move( uniforms_up ) );
    }

    // ---- ---- ---- ----

    inline void uniform_lap::auto_create_uniforms_up()
    {
      if ( !uniforms_up )
        uniforms_up = std::make_unique<uniform_container>();
    }

    // ---- ----

    template< class... Args >
    inline void uniform_lap::set_uniform( const std::string & var_name, Args... values )
    {
      auto_create_uniforms_up();
      uniforms_up->set_uniform( var_name, values... );
    }

    template< class... Args >
    inline void uniform_lap::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values )
    {
      auto_create_uniforms_up();
      uniforms_up->set_uniform_on_condition( condition_name, var_name, values... );
    }

  }
}

