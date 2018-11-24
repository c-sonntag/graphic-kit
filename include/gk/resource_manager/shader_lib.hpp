#pragma once

#include <resource_manager/abstract.hpp>

#include <erc/package_id.h>

#include <raiigl/shader.hpp>

#include <gk/opengl/quick_shader.h>

#include <memory>
#include <map>

namespace resource_manager {

  struct shader_lib : public abstract<raiigl::shader>
  {
   public:
    static const raiigl::shader & get_from_local( const std::string & lib_erc_path, const raiigl::shader_type type );

   public:
    erc::package_id package_id;

    shader_lib( const erc::package_id _package_id ) :
      package_id( std::move( _package_id ) ) {}

    shader_lib( const std::string _package_id_name ) :
      package_id( _package_id_name ) {}

    const raiigl::shader & get( const std::string & erc_path, const raiigl::shader_type type ) {
      return base_get( erc_path, [&]() -> std::unique_ptr<raiigl::shader>
      {
        return std::unique_ptr<raiigl::shader>(
          new raiigl::shader(
            gk::opengl::quick_shader::open_from_local_erc(
              package_id.from( erc_path ), type
            ) )
        );
        // return std::make_unique<raiigl::shader>(
        //   gk::opengl::quick_shader::open_from_local_erc(
        //     package_id.from( erc_path ), type
        //   ) );
      } );
    }

  };

}
