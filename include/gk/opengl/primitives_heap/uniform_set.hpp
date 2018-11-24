#pragma once

#include <raiigl/program.hpp>

#include <string>
#include <unordered_map>

#include <functional>

namespace gk {
  namespace opengl {

    // template<typename TUniform> /**< @todo */
    struct uniform_set
    {
     protected:
      const std::string name;
      // TUniform value; /**< @todo */
      const std::function<void( const raiigl::program & )> program_setter;

     public:
      template< class... Args >
      uniform_set( std::string _name, const Args & ...  args );

     public:
      void set( const raiigl::program & program ) const;
    };

    // ---- ----

    struct conditional_uniform_set
    {
     protected:
      std::unordered_map<std::string, bool> conditional_activation;

     public:
      void enable_set( const std::string & condition_name );
      void disable_set( const std::string & condition_name );
      bool is_set( const std::string & condition_name ) const;
    };

  }
}

#include <gk/opengl/primitives_heap/uniform_set.ipp>
