#include <gk/encoder/object.hpp>

#include <gk/istream.hpp>

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <ostream>

#include <regex>

#include <gk/debug.hpp>

std::ostream& operator <<( std::ostream& os, const std::smatch& match )
{
  os << "(";
  for( size_t i = 0; i < match.size(); ++i )
  {
    const std::string s( match[i].str() );
    os << "{" << i << "=" << s << "}";// << std::endl;
  }
  os << ")";
  return os;
}


namespace gk {
  namespace encoder {


    object object::load_from_file( const std::string& file_path, const object_importer_option& option )
    {
      try {
        std::ifstream ifs( file_path, std::ios_base::in );
        if( !ifs ) throw std::runtime_error( "Can't open file : " + file_path );
        return object( ifs, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::encoder::object::load_from_file] " + std::string( e.what() ) ); }
    }

    object object::load_from_memory( const std::string& input_data, const object_importer_option& option )
    {
      try {
        imemstream ims( input_data );
        return object( ims, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::encoder::object::load_from_memory] " + std::string( e.what() ) ); }
    }

    object object::load_from_stream( std::istream& is, const object_importer_option& option )
    {
      try {
        return object( is, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::encoder::object::load_from_stream] " + std::string( e.what() ) ); }
    }

    object object::load_from_erc( const erc::embedded_file& erc, const object_importer_option& option )
    {
      try {
        imemstream ims( erc.get_proper_data() );
        //return object( ims, option );
        debug_cout << "   load" << std::endl;
        object o( ims, option );
        debug_cout << "  return" << std::endl;
        return o;
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::encoder::object::load_from_erc] (file:" + erc.path + ") " + std::string( e.what() ) ); }
    }

    // ---- ---- ---- ----

    object::object( std::istream& is, const object_importer_option _option ) :
      option( std::move( _option ) )
    {
      //
      elements.emplace_back( *this );
      current_element = &elements.back();

      //
      uint line_counter( 0 );
      std::string line;
      while( std::getline( is, line ) )
      {
        ++line_counter;
        std::cout << line_counter << '\n';
        current_element->parse( line_counter, line );
      }

      debug_cout << "text parsed" << std::endl;

      //
      if( !option.do_not_finalize_element )
        for( auto& el : elements )
          el.finalise_it();

      debug_cout << "text finalized" << std::endl;
    }

    // ---- ---- ---- ----

    /**
     * @see https://stackoverflow.com/a/40466480
     */
    inline std::string strip_spaces( std::string line )
    {
      const auto have_no_space( []( char c ) { return !( ( c == ' ' ) || ( c == '\t' ) ); } );

      // ltrim
      auto ltrim_it( std::find_if( line.begin(), line.end(), have_no_space ) );
      line.erase( line.begin(), ltrim_it );

      // rtrim
      auto rtrim_it( std::find_if( line.rbegin(), line.rend(), have_no_space ) );
      line.erase( rtrim_it.base(), line.end() );

      //
      return line;
    }

    // ---- ----

    void object::element::parse( uint line_number, const std::string& line )
    {
#define cerr_parse std::cerr << "[gk::encoder::internal_make_object][line:" << line_number << "] "

      //
      static std::regex regex_line( R"(^[ \t]*([^#\n\r]+)?(?:[# ]+([^\n\r]*))?[ \n\r]*$)" );
      std::smatch match_line;
      if( std::regex_search( line, match_line, regex_line ) )
      {
        //
        const std::string command_line( match_line[1] );
        const std::string command_comment( match_line[2] );

        //
        static std::regex regex_command( R"(^([^ \t]+)[ \t]+(.+)$)" );

        //
        if( !command_comment.empty() && command_line.empty() )
        {
          std::smatch match_comment;
          if( std::regex_search( command_comment, match_comment, regex_command ) )
          {
            const std::string cmt( match_comment[1] );
            const std::string cmt_args( match_comment[2] );
            if( cmt == "object" ) { apply_name( ( cmt_args ) ); }
          }
        }
        else if( !command_line.empty() )
        {
          std::smatch match_command;
          if( !std::regex_search( command_line, match_command, regex_command ) )
          {
            cerr_parse << "Command need \"<cmd> <cmd_args>\" for : \"" << line << "\"" << std::endl;
            return;
          }

          //
          const std::string cmd( match_command[1] );
          const std::string cmd_args( strip_spaces( match_command[2] ) );

          //
          static const std::string strregex_op_float( R"((?:[-+]?[0-9]+)?(?:\.?(?:[0-9]+)?))" );
          static const std::string strregex_float( "(" + strregex_op_float + ")" );
          static const std::string strregex_op_int( R"((?:[-+]?[0-9]+))" );
          static const std::string strregex_int( "(" + strregex_op_int + ")" );
          static const std::string strregex_op_uint( R"([0-9]+)" );
          static const std::string strregex_uint( "(" + strregex_op_uint + ")" );

          //
          static const std::regex regex_coord2D( "^" + strregex_float + "[ \t]+" + strregex_float + "$" );
          static const std::regex regex_coord3D( "^" + strregex_float + "[ \t]+" + strregex_float + "[ \t]+" + strregex_float + "$" );

          //
          std::smatch match;

          //
          if( cmd == "g" )         { apply_name( cmd_args ); }

          //
          else if( cmd == "v" )
          {
            if( std::regex_search( cmd_args, match, regex_coord3D ) )
              vertices.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
          }
          else if( cmd == "vt" )
          {
            if( std::regex_search( cmd_args, match, regex_coord2D ) )
              texture2d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ) );
            else if( std::regex_search( cmd_args, match, regex_coord3D ) )
            {
              if( parent.option.force_texture2d_coords )
                texture2d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ) );
              else
                texture3d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
            }
          }
          else if( cmd == "vn" )
          {
            if( std::regex_search( cmd_args, match, regex_coord3D ) )
              normals.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
          }
          else if(  cmd == "f" )
          {
            //
            static const std::regex regex_face_block( R"(((?:(?:[0-9]+)\/?)+))" );
            static const std::regex regex_face_index( R"(([0-9]+))" );

            //
            uint vec_size_format( 0 );
            std::string::const_iterator search_start( cmd_args.cbegin() );
            while( regex_search( search_start, cmd_args.cend(), match, regex_face_block ) )
            {
              const std::string block( match[0] );
              search_start = match.suffix().first;

              //
              uint vec_size( 0 );
              std::smatch match_vec;
              std::string::const_iterator subsearch_start( block.cbegin() );
              while( regex_search( subsearch_start, block.cend(), match_vec, regex_face_index ) )
              {
                //
                ++vec_size;
                const std::string index_str( match_vec[0] );
                const ulong index( std::stoul( index_str ) );
                subsearch_start = match_vec.suffix().first;

                // cmd like : f v/vt/vn

                //
                if( vec_size == 1 )
                  vertex_indices.emplace_back( index );
                else if( vec_size == 2 )
                  tex_coord_indices.emplace_back( index );
                else if( vec_size == 3 )
                  normal_indices.emplace_back( index );

                //
                if( vec_size_format != 0 )
                  if( vec_size > vec_size_format )
                    cerr_parse << "Not constant face block" << std::endl;
              }

              //
              if( vec_size_format == 0 )
                vec_size_format = vec_size;
            }
          }

        }

      }
    }


    void object::element::finalise_it()
    {
      //
      use_texture3d = !texture3d_coords.empty();

      //
      have_indices =
        !vertex_indices.empty()
        || !tex_coord_indices.empty()
        || !normal_indices.empty();

      //
      if( have_indices )
      {
        //
        const bool have_vertex( vertex_indices.size() > 0 );
        const bool have_normal( normal_indices.size() > 0 );
        const bool have_tex2D( texture2d_coords.size() > 0 );
        const bool have_tex3D( texture3d_coords.size() > 0 );

        //
        if( !have_vertex )
          throw std::runtime_error( " object(" + name + ") have no vertices" );

        //
        if( have_tex2D && have_tex3D )
          throw std::runtime_error( " object(" + name + ") have texture2d_coords and texture3d_coords. Who choose ?" );

        //
        //static const auto substract_or_null([]( uint v ) { return ( v>0 ) ? ( v - 1 ) : v; } );

        //
        static const auto emplace_finalized_vec(
          []( auto& finalized, const auto& original, const uint index )
          {
            if( index < original.size() )
              if( index > 0 )
                finalized.emplace_back( original[index - 1] );
          }
        );

        //
        if( have_vertex && have_normal && ( have_tex2D || have_tex3D ) )
        {
          if( have_normal && ( vertex_indices.size() != normal_indices.size() ) )
            throw std::runtime_error( " object(" + name + ") not have same indices count vertex_indices(" + std::to_string( vertex_indices.size() ) + ") normal_indices(" + std::to_string( normal_indices.size() ) + ")" );
          else if( ( have_tex2D || have_tex3D ) && ( vertex_indices.size() != tex_coord_indices.size() ) )
            throw std::runtime_error( " object(" + name + ") not have same indices count vertex_indices(" + std::to_string( vertex_indices.size() ) + ") tex_coord_indices(" + std::to_string( tex_coord_indices.size() ) + ")" );
          // else if( have_tex3D && ( vertex_indices.size() != texture3d_coords.size() ) )
          //   throw std::runtime_error( " object(" + name + ") not have same indices count vertex_indices(" + std::to_string( vertex_indices.size() ) + ") texture3d_coords(" + std::to_string( texture3d_coords.size() ) + ")" );

          debug_cout << "   before finalize : " << vertex_indices.size() << std::endl;

          //
          for( uint i( 0 ); i<vertex_indices.size(); ++i )
          {
            emplace_finalized_vec( finalized_vertices, vertices, vertex_indices[i] );

            //
            if( have_normal )
              emplace_finalized_vec( finalized_normals, normals, normal_indices[i] );
            if( have_tex2D )
              emplace_finalized_vec( finalized_texture2d_coords, texture2d_coords, tex_coord_indices[i] );
            if( have_tex3D )
              emplace_finalized_vec( finalized_texture3d_coords, texture3d_coords, tex_coord_indices[i] );
          }
          debug_cout << "   after finalize" << std::endl;
        }

        is_finalized = true;
      }

    }


    void object::element::apply_name( const std::string _name )
    {
      //
      const std::string name( strip_spaces( _name ) );

      //
      bool find( false );
      for( auto& el : parent.elements )
      {
        if( el.name == name )
        {
          parent.current_element = &el;
          find = true;
          break;
        }
      }

      //
      if( find == false )
      {
        if( !parent.current_element->name.empty() )
        {
          parent.elements.emplace_back( parent );
          parent.current_element = &parent.elements.back();
        }

        //
        parent.current_element->name = name;
      }


    }


  }
}

