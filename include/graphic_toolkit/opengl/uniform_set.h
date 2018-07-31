#pragma once
#ifndef graphic_toolkit_opengl_uniform_set_hpp
#define graphic_toolkit_opengl_uniform_set_hpp

#include <raiigl/program.hpp>

#include <string>
#include <unordered_map>

#include <functional>

namespace graphic_toolkit {
  namespace opengl {

    template<typename TUniform>
    struct uniform_set
    {
     protected:
      const std::string name;
      TUniform value; /**< @todo */
      //const std::function<void( raiigl::program & )> program_setter;

     public:
      template< class... Args >
      uniform_set( std::string _name, Args && ...  args );

     public:
      void set( raiigl::program & program ) const;
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

#include <graphic_toolkit/opengl/uniform_set.ipp>
#endif
