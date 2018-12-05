#pragma once

#include <gk/types.hpp>

#include <tuple>
#include <vector>

namespace gk {
  namespace classes {

    /**
     * @brief The basic_setup struct
     * @example
     *
     * struct setup : public basic_setup<setup>
     * {
     *  public:
     *   optional_value<bool> option1{ *this, false };
     *
     *  public:
     *   optional_value<int, int> val2_default_init{ *this };
     *   optional_value<int, int> val2_init{ *this, 45, 76 };
     *
     *  public:
     *   setup() = default;
     * };
     *
     * inline void func()
     * {
     *   setup s( setup().option1( true ).val2_init( 42, 42 ).val2_default_init( 41, 89 ) );
     * }
     *
     */
    template<typename TClass>
    struct basic_setup
    {
      template<typename ... Args>
      struct optional_value
      {
       public:
        using values_tuple = std::tuple<Args ...>;

       private:
        bool is_set = false;
        values_tuple values;

       protected:
        friend basic_setup<TClass>;
        friend TClass;
        TClass& c;

        __forceinline optional_value( TClass& _c ) :
          c( _c ) { }

        template<typename ... SetArgs>
        __forceinline optional_value( TClass& _c, const SetArgs& ... args ) :
          optional_value( _c )
        { ( *this )( args ... ); }

       private:
        /**< @todo WHY values_tuple>::type NOT WORK ? */
        // template<uint P>
        // __forceinline void set( const typename std::tuple_element<P, values_tuple>::type& v )
        // { std::get<P>( values ) = v;}
        //
        // template<uint P, typename ... SetArgs>
        // __forceinline void set( const typename std::tuple_element<P, values_tuple>::type& v, const SetArgs& ... args )
        // { std::get<P>( values ) = v; set<P + 1>( args ... ); }
        template<uint P, typename T>
        __forceinline void set( const T& v )
        { std::get<P>( values ) = v;}

        template<uint P, typename T, typename ... SetArgs>
        __forceinline void set( const T& v, const SetArgs& ... args )
        { std::get<P>( values ) = v; set<P + 1>( args ... ); }

       public:
        __forceinline TClass& operator ()( const Args& ... args )
        { set<0>( args ... ); is_set = true; return c; }

        template<uint P, typename ... SetArgs>
        __forceinline TClass& operator ()( const SetArgs& ... variadic_args )
        { set<0>( variadic_args ... ); is_set = true; return c; }

       public:
        __forceinline operator bool() const { return is_set; }

       public:
        template<uint P>
        __forceinline const typename std::tuple_element<P, values_tuple>::type& get() const
        { return std::get<P>( values ); }

      };

    };





















  }
}

