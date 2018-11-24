#pragma once

#include <raiigl/gl_types.hpp>

#include <gk/opengl/primitives_heap/uniform_set.hpp>
#include <gk/opengl/primitives_heap/abstract_expander_property_support.hpp>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace gk {
  namespace opengl {

    struct uniform_container
    {
     public:
      using uniform_sets_t = std::list<uniform_set>;
      using uniform_sets_up_t = std::unique_ptr<uniform_sets_t>;
      using uniform_sets_conditional_t = std::unordered_map<std::string, uniform_sets_t>;
      using uniform_sets_conditional_up_t = std::unique_ptr<uniform_sets_conditional_t>;

     public:
      uniform_sets_up_t uniform_sets_up;
      uniform_sets_conditional_up_t uniform_sets_conditional_up;

     public:
      void apply_uniform_sets( const abstract_expander_property_support & bd, raiigl::program & program ) const;

     public:
      template< class... Args >
      void set_uniform( const std::string & var_name, const Args & ... values );

      //template< class... Args >
      //void set_uniform( const GLint id, const Args & ... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, const Args & ... values );

     // template< class... Args >
     // void set_uniform_on_condition( const std::string & condition_name, const GLint id, const Args & ... values );

    };

  }
}


#include <gk/opengl/primitives_heap/uniform_container.ipp>
