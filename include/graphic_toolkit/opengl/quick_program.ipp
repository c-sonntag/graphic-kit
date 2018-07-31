#pragma once

#include <graphic_toolkit/opengl/quick_program.h>

#include <erc/inventory_package.h>

#include <stdexcept>
#include <fstream>

namespace graphic_toolkit {
  namespace opengl {


    inline raiigl::program quick_program::open_from_sources( const std::string & vertex_src, const std::string & fragment_src )
    {
      return raiigl::program(
               raiigl::shader( vertex_src, raiigl::shader_type::Vertex ),
               raiigl::shader( fragment_src, raiigl::shader_type::Fragment )
             );
    }

    inline raiigl::program quick_program::open_from_file_path( const std::string & vertex_path, const std::string & fragment_path )
    {
      //
      std::ifstream vertex_ifs( vertex_path, std::ifstream::in );
      std::ifstream fragment_ifs( fragment_path, std::ifstream::in );

      //
      if ( !vertex_ifs || !fragment_ifs )
      {
        std::string files_errors;
        if ( !vertex_ifs ) files_errors += "\"" + vertex_path + "\"";
        if ( !fragment_ifs ) files_errors += std::string( ( !vertex_ifs ) ? " and " : "" ) + "\"" + fragment_path + "\"";
        throw std::runtime_error( "[graphic_toolkit::opengl::quick_program] Can't open file(s) : " + files_errors );
      }

      //
      return raiigl::program(
               raiigl::shader( vertex_ifs, raiigl::shader_type::Vertex ),
               raiigl::shader( fragment_ifs, raiigl::shader_type::Fragment )
             );
    }

    inline raiigl::program quick_program::open_from_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id )
    {
      //
      const erc::embedded_file * const vertex_erc( erc::inventory_package::get_first_embedded_file( vertex_erc_id ) );
      const erc::embedded_file * const fragment_erc( erc::inventory_package::get_first_embedded_file( fragment_erc_id ) );

      //
      if ( !vertex_erc || !fragment_erc )
      {
        std::string files_errors;
        if ( !vertex_erc ) files_errors += "\"" + vertex_erc_id.package_name + ":" + vertex_erc_id.file_path + "\"";
        if ( !fragment_erc ) files_errors += std::string( ( !vertex_erc ) ? " and " : "" ) + "\"" + fragment_erc_id.package_name + ":" + fragment_erc_id.file_path + "\"";
        throw std::runtime_error( "[graphic_toolkit::opengl::quick_program] Can't open embedded resource file(s) : " + files_errors );
      }

      //
      return raiigl::program(
               raiigl::shader( vertex_erc->get_proper_data(), raiigl::shader_type::Vertex ),
               raiigl::shader( fragment_erc->get_proper_data(), raiigl::shader_type::Fragment )
             );
    }



  }
}

