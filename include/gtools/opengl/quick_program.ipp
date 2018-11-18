#pragma once

#include <gtools/opengl/quick_program.hpp>

#include <erc/inventory_package.h>

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

namespace gtools {
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
        throw std::runtime_error( "[gtools::opengl::quick_program] Can't open file(s) : " + files_errors );
      }

      //
      return raiigl::program(
               vertex_path + "|" + fragment_path,
               raiigl::shader( vertex_ifs, raiigl::shader_type::Vertex ),
               raiigl::shader( fragment_ifs, raiigl::shader_type::Fragment )
             );
    }

    inline raiigl::program quick_program::open_from_erc( const erc::embedded_file & vertex_erc, const erc::embedded_file & fragment_erc )
    {
      //
      try
      {
        //
        raiigl::program shaders_program(
          vertex_erc.path + " | " + fragment_erc.path,
          raiigl::shader( vertex_erc.get_proper_data(), raiigl::shader_type::Vertex ),
          raiigl::shader( fragment_erc.get_proper_data(), raiigl::shader_type::Fragment )
        );

        //
        return shaders_program;
      }
      catch ( const std::exception & e )
      {
        std::stringstream ss;
        ss << "For program :" << std::endl
           << " - vertex(" << vertex_erc.path << ")" << std::endl
           << " - fragment(" << fragment_erc.path << ")" << std::endl
           << e.what();
        throw std::runtime_error( ss.str( ) );
      }
    }

    __forceinline raiigl::program quick_program::open_from_local_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id )
    {
      //
      const erc::embedded_file * const vertex_erc_p( erc::inventory_package::get().get_first_embedded_file( vertex_erc_id ) );
      const erc::embedded_file * const fragment_erc_p( erc::inventory_package::get().get_first_embedded_file( fragment_erc_id ) );

      //
      if ( !vertex_erc_p || !fragment_erc_p )
      {
        std::string files_errors;
        if ( !vertex_erc_p ) files_errors += "\"" + vertex_erc_id.package_name + ":" + vertex_erc_id.file_path + "\"";
        if ( !fragment_erc_p ) files_errors += std::string( ( !vertex_erc_p ) ? " and " : "" ) + "\"" + fragment_erc_id.package_name + ":" + fragment_erc_id.file_path + "\"";
        throw std::runtime_error( "[gtools::opengl::quick_program] Can't open embedded resource file(s) : " + files_errors );
      }

      //
      try
      {
        raiigl::program shaders_program(
          vertex_erc_id.to_string() + " | " + fragment_erc_id.to_string(),
          raiigl::shader( vertex_erc_p->get_proper_data(), raiigl::shader_type::Vertex ),
          raiigl::shader( fragment_erc_p->get_proper_data(), raiigl::shader_type::Fragment )
        );

        //
        vertex_erc_p->unallocate_proper_data();
        fragment_erc_p->unallocate_proper_data();

        //
        return shaders_program;
      }
      catch ( const std::exception & e )
      {
        std::stringstream ss;
        ss << "For program :" << std::endl
           << " - vertex(" << vertex_erc_id.to_string() << ")" << std::endl
           << " - fragment(" << fragment_erc_id.to_string() << ")" << std::endl
           << e.what();
        throw std::runtime_error( ss.str( ) );
      }
    }


  }
}

