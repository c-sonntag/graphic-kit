#pragma once

#include <gtk/opengl/primitives_heap/abstract_expander_property_support.hpp>

#include <memory>
#include <string>

namespace gtk {
  namespace opengl {

    template<typename  ... TListTypes>
    class primitives_heap;

    // ---- ----

    struct uniform_lap
    {
     protected:
      abstract_expander_property_support & expander_support;

     protected:
      abstract_expander_property_support::uniform_container_up_t uniforms_up;

     protected:
      template<typename  ... TListTypes>
      friend class primitives_heap;

      //friend primitives_heap<TListTypes...>;
      uniform_lap( abstract_expander_property_support & _expander_support );

     public:
      // Enable move.
      uniform_lap( uniform_lap && ) = default;
      // Disable copy from lvalue.
      uniform_lap( const uniform_lap & ) = delete;
      uniform_lap & operator=( const uniform_lap & ) = delete;

     public:
      ~uniform_lap();

     private:
      inline void auto_create_uniforms_up();

     public:
      template< class... Args >
      void set_uniform( const std::string & var_name, Args... values );

      template< class... Args >
      void set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values );


    };

  }
}


#include <gtk/opengl/primitives_heap/uniform_lap.ipp>
