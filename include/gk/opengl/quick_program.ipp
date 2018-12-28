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
        throw std::runtime_error( "[create_shader] from embedded_file : \"" + erc.path + "\" : " + std::string(e.what() ) );
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
        throw std::runtime_error( "[create_shader] from file_id : \"" + erc_file_id.to_string() + "\" : " + std::string(e.what() ) );
      }
    }

    // ---- ----

    template<typename TAssoc>
    __forceinline void quick_program::program_composition::open_from( shader_file_assoc<TAssoc> shader_assoc )
    {
      try {
        shaders.emplace_back( open_from_create_shader( std::move( shader_assoc ) ) );
      } catch ( const std::exception& e ) {
        shaders_creation_errors.emplace_back( "[" + raiigl::name( shader_assoc.type ) + "Shader]" + std::string( e.what() ) );
      }
    }

    template<typename TAssoc, typename ... Args>
    __forceinline void quick_program::program_composition::open_from( shader_file_assoc<TAssoc> shader_assoc, Args&& ... args )
    {
      open_from( std::move( shader_assoc ) );
      open_from( std::forward<Args>( args ) ... );
    }

    // ---- ---- ---- ----

    template<typename ... Args>
    __forceinline quick_program::program_composition::program_composition( Args && ... args )
    {
      open_from( std::forward<Args>( args ) ... );
      check_errors();

    }

    __forceinline quick_program::program_composition::~program_composition()
    {
      //
      for( const erc::embedded_file* erc_file_p : openned_embedded_files_p )
        if( erc_file_p )
          erc_file_p->unallocate_proper_data();

    }

    // ---- ---- ---- ----

    __forceinline raiigl::program quick_program::program_composition::made_program() const
    {
      //
      if( shaders.empty() )
        throw std::runtime_error( "[raiigl::program quick_program::program_composition::made_program] No shaders" );

      //
      std::string program_name;
      for( const shader_comp& sc : shaders )
      {
        if( !program_name.empty() )
          program_name += " | ";
        program_name += raiigl::name( sc.type ) + "(" + sc.composition_name + ")";
      }

      //
      raiigl::program::shaders_p_t shaders_p;
      for( const shader_comp& sc : shaders )
        shaders_p.emplace( &dynamic_cast<const raiigl::shader&>( sc ) );

      //
      return raiigl::program( program_name, shaders_p );
    }

  }
}

