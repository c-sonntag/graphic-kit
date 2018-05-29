#pragma once
#ifndef graphic_toolkit_opengl_uniform_lap_h
#define graphic_toolkit_opengl_uniform_lap_h

#include <graphic_toolkit/opengl/abstract_expander_property_support.h>

#include <memory>
#include <string>

namespace graphic_toolkit {
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


#include <graphic_toolkit/opengl/uniform_lap.ipp>
#endif
