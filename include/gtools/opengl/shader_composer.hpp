#pragma once

#include <raiigl/classes/non_copyable_movable.hpp>
#include <types.hpp>

#include <raiigl/gl430.hpp>
#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>

#include <shader_lib/abstract.hpp>

#include <debug.hpp>

struct shader_composer
{
  static int debug_count;
#define PAD std::string( debug_count, ' ' )

 protected:
  inline void deploy( const raiigl::shader & shader )
  { shaders.emplace( &shader ); }

  inline void deploy( const raiigl::program::shaders_p_t & shaders_p ) {
    for ( const raiigl::shader * const shader_p : shaders_p )
      if ( shader_p ) shaders.emplace( shader_p );
  }

  inline void deploy( const shader_composer & compose )
  { deploy( compose.shaders ); }

  inline void deploy( const shader_lib::abstract & shader_lib ) {
    deploy( shader_lib.lib );
    // deploy( shader_lib.libs_depends ); /** @todo and depends ? */
  }

  inline void deploy( const shader_lib::collection & lib_deps ) {
    for ( const std::shared_ptr< shader_lib::abstract> & lib_dep_up : lib_deps )
      if ( lib_dep_up ) deploy( *lib_dep_up );
  }

 protected:
  //template<class ... TArgs>
  //__forceinline void deploy( const shader_lib_collections & lib_deps, const TArgs & ... args ) {
  //  deploy( lib_deps );
  //  deploy( args... );
  //}
  //
  //template<class ... TArgs>
  //__forceinline void deploy( const raiigl::shader & shader, const TArgs & ... args ) {
  //  deploy( shader );
  //  deploy( args... );
  //}

  template<class TArg>
  inline void base_deploy( const TArg & arg ) {
    debug_count++;
    debug_cout << PAD << " base_deploy1 " << std::endl;
    deploy( arg );
    debug_count--;
  }

  template<class TArg, class ... TArgs>
  inline void base_deploy( const TArg & arg, const TArgs & ... args ) {
    debug_count++;
    debug_cout << PAD << " base_deploy2 " << std::endl;
    deploy( arg );
    base_deploy( args... );
    debug_count--;
  }

 public:
  raiigl_classes_non_copyable_movable( shader_composer )

  //template<class ... TArgs>
  //static __forceinline void deploy( const shader_compose & compose, const TArgs & ... args ) {
  //  deploy( compose );
  //  deploy( args... );
  //}

 public:
  raiigl::program::shaders_p_t shaders;

 public:
  template<class ... TArgs>
  explicit __forceinline shader_composer( const TArgs & ... args ) {
    debug_cout << PAD << "shadercompose size : " << sizeof...( args ) << std::endl;

    base_deploy( args... );

    debug_cout << PAD << "construct( ";
    for ( const raiigl::shader * const s : shaders )
      debug_cout << s << "(id:" << s->id << ") ";
    debug_cout << ")" << std::endl;
  }

  __forceinline ~shader_composer() {
    debug_cout << PAD << "destruct( ";
    for ( const raiigl::shader * const s : shaders )
      debug_cout << s << "(id:" << s->id << ") ";
    debug_cout << ")" << std::endl;
  }

};

