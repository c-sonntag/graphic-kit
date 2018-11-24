#pragma once

#include <gk/opengl/quick_shader.h>

#include <erc/inventory_package.h>

#include <stdexcept>
#include <fstream>

namespace gk {
  namespace opengl {


    inline raiigl::shader quick_shader::open_from_file_path( const std::string & shader_path, const raiigl::shader_type type )
    {
      //
      std::ifstream shader_ifs( shader_path, std::ifstream::in );

      //
      if ( !shader_ifs )
        throw std::runtime_error( "[gk::opengl::quick_shader] Can't open shader file  : " + shader_path );

      //
      return raiigl::shader( shader_ifs, type );
    }

    inline raiigl::shader quick_shader::open_from_erc( const erc::embedded_file & shader_erc, const raiigl::shader_type type )
    {
      raiigl::shader shaders_shader( shader_erc.get_proper_data(), type );
      return shaders_shader;
    }

    __forceinline raiigl::shader quick_shader::open_from_local_erc( const erc::file_id & shader_erc_id, const raiigl::shader_type type )
    {
      //
      const erc::embedded_file * const shader_erc_p( erc::inventory_package::get().get_first_embedded_file( shader_erc_id ) );

      //
      if ( !shader_erc_p )
        throw std::runtime_error( "[gk::opengl::quick_shader] Can't open embedded resource file : \"" + shader_erc_id.package_name + ":" + shader_erc_id.file_path + "\"" );

      //
      raiigl::shader shaders_shader( open_from_erc( *shader_erc_p, type ) );
      shader_erc_p->unallocate_proper_data();

      //
      return shaders_shader;
    }


  }
}

