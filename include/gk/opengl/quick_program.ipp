#pragma once

#include <gk/opengl/quick_program.hpp>

#include <erc/inventory_package.h>

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace gk {
  namespace opengl {


    inline raiigl::shader_type quick_program::detect_type( const std::string& extension )
    {
      static std::multimap<std::string, raiigl::shader_type> extension_shaders
      {
        {{".glsl", ".gsh"}, raiigl::shader_type::Unknown},
        {{".gsh"}, raiigl::shader_type::Geometry},
        {{".vert", ".vsh"}, raiigl::shader_type::Vertex, },
        {{".frag", ".fsh"}, raiigl::shader_type::Fragment, },
        {{".comp", ".compute"}, raiigl::shader_type::Compute },
      };
      const auto type_find_it( extension_shaders.find( extension ) );
      if( type_find_it == extension_shaders.end() )
        return raiigl::shader_type::Unknown;
      return type_find_it->second;
    }

    // ---- ----

    //
    //  __forceinline quick_program::shader_file_assoc<std::string> quick_program::shader_from_file_path( const std::string& file_path )
    //  { return {detect_type(file_path)}; }
    //
    //  __forceinline quick_program::shader_file_assoc<erc::file_id> quick_program::shader_from_local_erc( const erc::file_id& file_id )
    //  {}
    //
    // ---- ---- ---- ----

    __forceinline void quick_program::program_composition::check_errors()
    {
      if( !shaders_creation_errors.empty() )
      {
        std::stringstream ss;
        ss << "[gk::opengl::quick_program::program_composition]" << std::endl
           << "  " << shaders_creation_errors.size() << " error(s) : " << std::endl;
        for( const std::string& s : shaders_creation_errors )
          ss << "  - " << s << std::endl;
        ss << std::endl;
        throw std::runtime_error( ss.str() );
      }
    }

    __forceinline void quick_program::program_composition::close()
    {
      //
      for( const erc::embedded_file* erc_file_p : openned_embedded_files_p )
        if( erc_file_p )
          erc_file_p->unallocate_proper_data();

    }

    // ---- ----

    template<typename TAssoc>  __forceinline
    quick_program::program_composition::shader_comp
    quick_program::program_composition::open_from_create_shader( shader_file_assoc<TAssoc> shader_assoc )
    { return shader_comp( shader_assoc.assoc, shader_assoc.type  ).set_composition_name( "unknown_assoc_type(" + std::string( typeid( TAssoc ).name() ) + ")" );  }

    __forceinline quick_program::program_composition::shader_comp
    quick_program::program_composition::open_from_create_shader( quick_program::shader_file_assoc<erc::embedded_file> shader_assoc )
    {
      const erc::embedded_file& erc( shader_assoc.assoc );
      try
      { return shader_comp( erc.get_proper_data(), shader_assoc.type ).set_composition_name( erc.path ); }
      catch ( const std::exception& e ) {
        throw std::runtime_error( "[Shader] Can't create program for : \"" + erc.path + "\" : " + std::string(e.what() ) );
      }
    }

    __forceinline quick_program::program_composition::shader_comp
    quick_program::program_composition::open_from_create_shader( quick_program::shader_file_assoc<erc::file_id> shader_assoc )
    {
      const erc::file_id& erc_file_id( shader_assoc.assoc );
      const erc::embedded_file* const erc_file_p( erc::inventory_package::get().get_first_embedded_file( shader_assoc.assoc ) );
      //
      if( !erc_file_p  )
        throw std::runtime_error( "[ErcFile] Can't open embedded resource file : \"" + erc_file_id.to_string() + "\"" );
      //
      try
      {
        openned_embedded_files_p.emplace_back( erc_file_p );
        return shader_comp( erc_file_p->get_proper_data(), shader_assoc.type ).set_composition_name( erc_file_id.to_string() );
      }
      catch ( const std::exception& e ) {
        throw std::runtime_error( "[Shader] Can't create program for : \"" + erc_file_id.to_string() + "\" : " + std::string(e.what() ) );
      }
    }

    // ---- ----

    //  template<typename ... Args> __forceinline
    //  raiigl::program quick_program::open_from_local_erc_deploy( erc::file_id file_id, Args ... args )
    //  {
    //    std::list<erc::file_id> file_ids;
    //    open_from_local_erc_deploy( file_ids, std::move( file_id ), args ... );
    //
    //  }

    // ---- ----

    template<typename TAssoc>
    __forceinline raiigl::program quick_program::program_composition::open_from( shader_file_assoc<TAssoc> shader_assoc )
    {
      try {
        shaders.emplace( open_from_create_shader( shader_assoc.assoc ) );
      } catch ( const std::exception& e ) {
        shaders_creation_errors.emplace_back( "[" + raiigl::name( shader_assoc.type ) + "Shader]" + std::string( e.what() ) );
      }
    }

    template<typename TAssoc, typename ... Args>
    __forceinline raiigl::program quick_program::program_composition::open_from( shader_file_assoc<TAssoc> shader_assoc, Args ... args )
    {
      open_from( shaders, std::move( shader_assoc ) );
      open_from( shaders, std::move( args ... ) );
    }

    // ---- ---- ---- ----


    //    inline raiigl::program quick_program::open_from_sources( const std::string& vertex_src, const std::string& fragment_src )
    //    {
    //      return raiigl::program(
    //        raiigl::shader( vertex_src, raiigl::shader_type::Vertex ),
    //        raiigl::shader( fragment_src, raiigl::shader_type::Fragment )
    //      );
    //    }

    //    inline raiigl::program quick_program::open_from_file_path( const std::string& vertex_path, const std::string& fragment_path )
    //    {
    //      //
    //      std::ifstream vertex_ifs( vertex_path, std::ifstream::in );
    //      std::ifstream fragment_ifs( fragment_path, std::ifstream::in );

    //      //
    //      if( !vertex_ifs || !fragment_ifs )
    //      {
    //        std::string files_errors;
    //        if( !vertex_ifs ) files_errors += "\"" + vertex_path + "\"";
    //        if( !fragment_ifs ) files_errors += std::string( ( !vertex_ifs ) ? " and " : "" ) + "\"" + fragment_path + "\"";
    //        throw std::runtime_error( "[gk::opengl::quick_program] Can't open file(s) : " + files_errors );
    //      }

    //      //
    //      return raiigl::program(
    //        vertex_path + "|" + fragment_path,
    //        raiigl::shader( vertex_ifs, raiigl::shader_type::Vertex ),
    //        raiigl::shader( fragment_ifs, raiigl::shader_type::Fragment )
    //      );
    //    }

    // ----



















    //    inline raiigl::program quick_program::open_from_erc( const erc::embedded_file& vertex_erc, const erc::embedded_file& fragment_erc )
    //    {
    //      //
    //      try
    //      {
    //        //
    //        raiigl::program shaders_program(
    //          vertex_erc.path + " | " + fragment_erc.path,
    //          raiigl::shader(vertex_erc.get_proper_data(), raiigl::shader_type::Vertex),
    //          raiigl::shader(fragment_erc.get_proper_data(), raiigl::shader_type::Fragment)
    //        );

    //        //
    //        return shaders_program;
    //      }
    //      catch ( const std::exception& e )
    //      {
    //        std::stringstream ss;
    //        ss << "For program :" << std::endl
    //           << " - vertex(" << vertex_erc.path << ")" << std::endl
    //           << " - fragment(" << fragment_erc.path << ")" << std::endl
    //           << e.what();
    //        throw std::runtime_error( ss.str() );
    //      }
    //    }

    // ---- ---- ---- ----

    //    __forceinline raiigl::program quick_program::open_from_local_erc( const erc::file_id& vertex_erc_id, const erc::file_id& fragment_erc_id )
    //    {
    //      //
    //      const erc::embedded_file* const vertex_erc_p( erc::inventory_package::get().get_first_embedded_file( vertex_erc_id ) );
    //      const erc::embedded_file* const fragment_erc_p( erc::inventory_package::get().get_first_embedded_file( fragment_erc_id ) );

    //      //
    //      if( !vertex_erc_p || !fragment_erc_p )
    //      {
    //        std::string files_errors;
    //        if( !vertex_erc_p ) files_errors += "\"" + vertex_erc_id.package_name + ":" + vertex_erc_id.file_path + "\"";
    //        if( !fragment_erc_p ) files_errors += std::string( ( !vertex_erc_p ) ? " and " : "" ) + "\"" + fragment_erc_id.package_name + ":" + fragment_erc_id.file_path + "\"";
    //        std::stringstream ss;
    //        ss << "[gk::opengl::quick_program] Can't open embedded resource file(s) : " << files_errors << std::endl
    //           << "See files of current inventory : " << std::endl;
    //        erc::inventory_package::get().debug_print( ss );
    //        throw std::runtime_error( ss.str() );
    //      }

    //      //
    //      try
    //      {
    //        raiigl::program shaders_program(
    //          vertex_erc_id.to_string() + " | " + fragment_erc_id.to_string(),
    //          raiigl::shader(vertex_erc_p->get_proper_data(), raiigl::shader_type::Vertex),
    //          raiigl::shader(fragment_erc_p->get_proper_data(), raiigl::shader_type::Fragment)
    //        );

    //        //
    //        vertex_erc_p->unallocate_proper_data();
    //        fragment_erc_p->unallocate_proper_data();

    //        //
    //        return shaders_program;
    //      }
    //      catch ( const std::exception& e )
    //      {
    //        std::stringstream ss;
    //        ss << "For program :" << std::endl
    //           << " - vertex(" << vertex_erc_id.to_string() << ")" << std::endl
    //           << " - fragment(" << fragment_erc_id.to_string() << ")" << std::endl
    //           << e.what();
    //        throw std::runtime_error( ss.str() );
    //      }
    //    }

    // ---- ----
    // ---- ----

    // __forceinline raiigl::program quick_program::open_from_local_erc( const std::vector<erc::file_id>& file_ids )
    // {
    //
    // }


    // ---- ---- ---- ----



    // ---- ---- ---- ----


    // template<typename ... Args> __forceinline
    // raiigl::program quick_program::open_from_local_erc_deploy( erc::file_id file_id, Args ... args )
    // {
    //   std::list<erc::file_id> file_ids;
    //   open_from_local_erc_deploy( file_ids, std::move( file_id ), args ... );
    //
    // }
    //
    // template<typename ... Args> __forceinline
    // raiigl::program quick_program::open_from_local_erc_deploy( std::list<erc::file_id>& file_ids, erc::file_id file_id, Args ... args )
    // { open_from_local_erc_deploy( file_ids, std::move( file_id ) ); open_from_local_erc_deploy( file_ids, std::move( args ... ) ); }
    //
    // __forceinline raiigl::program quick_program::open_from_local_erc_deploy( std::list<erc::file_id>& file_ids, erc::file_id file_id )
    // { file_ids.emplace_back( std::move( file_id ) ); }


    // ---- ---- ---- ----

    // template<typename ... Args>
    // __forceinline raiigl::program quick_program::open_from_local_erc( Args ... args )
    // { return open_from_local_erc_deploy( args ... ); }



  }
}

