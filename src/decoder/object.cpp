#include <gk/decoder/object.hpp>

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
    os << "{" << i << "=" << s << "}";// << '\n';
  }
  os << ")";
  return os;
}

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

namespace gk {
  namespace decoder {


    object object::load_from_file( const std::string& file_path, const object_importer_option& option )
    {
      try {
        std::ifstream ifs( file_path, std::ios_base::in );
        if( !ifs ) throw std::runtime_error( "Can't open file : " + file_path );
        return object( ifs, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::decoder::object::load_from_file] " + std::string( e.what() ) ); }
    }

    object object::load_from_memory( const char* const data, const uint size, const object_importer_option& option )
    {
      try {
        imemstream ims( data, size );
        return object( ims, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::decoder::object::load_from_memory] " + std::string( e.what() ) ); }
    }

    object object::load_from_stream( std::istream& is, const object_importer_option& option )
    {
      try {
        return object( is, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::decoder::object::load_from_stream] " + std::string( e.what() ) ); }
    }

    object object::load_from_erc( const erc::embedded_file& erc, const object_importer_option& option )
    {
      try {
        imemstream ims( erc.get_proper_data() );
        return object( ims, option );
      }
      catch( const std::exception& e )
      { throw std::runtime_error( "[gk::decoder::object::load_from_erc] (file:" + erc.path + ") " + std::string( e.what() ) ); }
    }

    // ---- ---- ---- ----

    object::object( std::istream& is, const object_importer_option& _option ) :
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
        current_element->parse( line_counter, line );
      }

      //
      use_texture3d = !texture3d_coords.empty();

      //
      for( auto& el : elements )
        el.close();
    }

    // ---- ---- ---- ----

    object::element::finalized::finalized( const element& _el ) :
      el( _el )
    {
      //
      if( el.have_indices )
      {
        //
        if( el.have_quad_face )
          throw std::runtime_error( " object(" + el.name + ") need triangle face, not quad, please option 'convert_quad_face_to_quad_triangle' in object importer" );

        //
        const bool have_vertex( !el.vertex_indices.empty() && ( el.face_vec_size_format >= 1 ) );
        const bool have_tex2D( !el.parent.texture2d_coords.empty() && ( el.face_vec_size_format >= 2 ) );
        const bool have_normal( !el.normal_indices.empty() && ( el.face_vec_size_format >= 3 ) );
        // Maybe ? const bool have_tex3D( !el.parent.texture3d_coords.empty() && ( el.face_vec_size_format >= 4 ) );

        //
        if( !have_vertex )
          throw std::runtime_error( " object(" + el.name + ") have no vertices" );
        // if( have_tex2D && have_tex3D )
        //   throw std::runtime_error( " object(" + el.name + ") have texture2d_coords and texture3d_coords. Who choose ?" );

        //
        static const auto emplace_vec(
          []( auto& finalized, const auto& original, const uint index )
          {
            if( index <= original.size() )
              if( index > 0 )
                finalized.emplace_back( original[index - 1] );
          }
        );

        //
        if( have_vertex )
        {
          if( have_normal && ( el.vertex_indices.size() != el.normal_indices.size() ) )
            throw std::runtime_error( " object(" + el.name + ") not have same indices count vertex_indices(" + std::to_string( el.vertex_indices.size() ) + ") normal_indices(" + std::to_string( el.normal_indices.size() ) + ")" );
          else if( ( have_tex2D /*|| have_tex3D*/ ) && ( el.vertex_indices.size() != el.tex_coord_indices.size() ) )
            throw std::runtime_error( " object(" + el.name + ") not have same indices count vertex_indices(" + std::to_string( el.vertex_indices.size() ) + ") tex_coord_indices(" + std::to_string( el.tex_coord_indices.size() ) + ")" );

          //
          const uint vertices_indices_count( static_cast<uint>( el.vertex_indices.size() ) );
          vertices.reserve( vertices_indices_count );

          if( have_tex2D )
            texture2d_coords.reserve( vertices_indices_count );
          if( have_normal )
            normals.reserve( vertices_indices_count );
          // if( have_tex3D )
          //   texture3d_coords.reserve( vertices_indices_count );

          //
          for( uint i( 0 ); i<vertices_indices_count; ++i )
          {
            emplace_vec( vertices, el.parent.vertices, el.vertex_indices[i] );
            //
            if( have_tex2D )
              emplace_vec( texture2d_coords, el.parent.texture2d_coords, el.tex_coord_indices[i] );
            if( have_normal )
              emplace_vec( normals, el.parent.normals, el.normal_indices[i] );
            // if( have_tex3D )
            //   emplace_vec( texture3d_coords, el.parent.texture3d_coords, el.tex_coord_indices[i] );
          }
        }
      }
    }


    // ---- ---- ---- ----

    void object::element::close()
    {
      //
      have_indices =
        !vertex_indices.empty()
        || !tex_coord_indices.empty()
        || !normal_indices.empty();

      //
      if( parent.option.force_finalize_element )
        get_finalized();
    }

    // ---- ----

    const object::element::finalized& object::element::get_finalized()
    {
      if( !m_finalized_up )
        m_finalized_up = std::unique_ptr<finalized>( new finalized( *this ) );
      return *m_finalized_up;
    }

    // ---- ----

    void object::element::apply_name( const std::string name )
    {

      //
      bool import = true;

      //
      if( !name.empty() )
      {
        if( !parent.option.not_import_element_name.empty() )
        {
          const bool not_import( parent.option.not_import_element_name.find( name ) != parent.option.not_import_element_name.end() );
          if( not_import ) import = false;
        }
        if( !parent.option.only_import_element_name.empty() )
        {
          const bool only_import( parent.option.only_import_element_name.find( name ) != parent.option.only_import_element_name.end() );
          if( !only_import ) import = false;
        }
      }

      //
      if( !import )
      {
        stop_import_face = true;
        return;
      }
      else if( stop_import_face )
      {
        stop_import_face = false;
      }

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
          // std::smatch match_comment;
          // if( std::regex_search( command_comment, match_comment, regex_command ) )
          // {
          //   const std::string cmt( match_comment[1] );
          //   const std::string cmt_args( match_comment[2] );
          //   // if( cmt == "object" ) { apply_name( ( cmt_args ) ); } not need comment flag
          // }
        }
        else if( !command_line.empty() )
        {
          std::smatch match_command;
          if( !std::regex_search( command_line, match_command, regex_command ) )
          {
            cerr_parse << "Command need \"<cmd> <cmd_args>\" for : \"" << line << "\"" << '\n';
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
          uint face_vec_format( 0 );

          //
          if( cmd == "g" )         { apply_name( cmd_args ); }

          //
          else if( cmd == "v" )
          {
            if( std::regex_search( cmd_args, match, regex_coord3D ) )
              parent.vertices.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
          }
          else if( cmd == "vt" )
          {
            if( std::regex_search( cmd_args, match, regex_coord2D ) )
              parent.texture2d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ) );
            else if( std::regex_search( cmd_args, match, regex_coord3D ) )
            {
              if( parent.option.force_texture2d_coords )
                parent.texture2d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ) );
              else
                parent.texture3d_coords.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
            }
          }
          else if( cmd == "vn" ) // cmd like : vn n1/n2/n3
          {
            if( std::regex_search( cmd_args, match, regex_coord3D ) )
              parent.normals.emplace_back( std::stof( match[1] ), std::stof( match[2] ), std::stof( match[3] ) );
          }
          else if(  cmd == "f" && !stop_import_face )  // cmd like : f v/vt/vn v/vt/vn v/vt/vn
          {
            //
            static const std::regex regex_face_block( R"(((?:(?:[0-9]+)\/?)+))" );
            static const std::regex regex_face_index( R"(([0-9]+))" );

            //
            uint actual_block_count( 0 );
            std::string::const_iterator search_start( cmd_args.cbegin() );
            while( regex_search( search_start, cmd_args.cend(), match, regex_face_block ) )
            {
              ++actual_block_count;
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

                if( ( actual_block_count == 4 ) && parent.option.convert_quad_face_to_quad_triangle )
                {
                  //
                  static auto emplace_index_from_tri(
                    []( auto& vec, const uint index ) {
                      const std::size_t size( vec.size() );
                      if( size >= 3 )
                      {
                        vec.emplace_back( vec[size - 1] );
                        vec.emplace_back( index );
                        vec.emplace_back( vec[size - 3] );
                      }
                    }
                  );
                  if( vec_size == 1 )
                    emplace_index_from_tri( vertex_indices, index );
                  else if( vec_size == 2 )
                    emplace_index_from_tri( tex_coord_indices, index );
                  else if( vec_size == 3 )
                    emplace_index_from_tri( normal_indices, index );
                }
                else
                {
                  if( vec_size == 1 )
                    vertex_indices.emplace_back( index );
                  else if( vec_size == 2 )
                    tex_coord_indices.emplace_back( index );
                  else if( vec_size == 3 )
                    normal_indices.emplace_back( index );
                }

                //
                if( face_vec_size_format != 0 )
                  if( vec_size > face_vec_size_format )
                    cerr_parse << "Not constant face block" << '\n';

                if( vec_size > 3 )
                { cerr_parse << "Incorrect face block actual=" << vec_size << "  need={1-3}" << '\n'; }
              }

              //
              if( face_vec_size_format == 0 )
                face_vec_size_format = vec_size;
            }

            //
            if( actual_block_count > 4 || actual_block_count < 3 )
            { cerr_parse << "Incorrect face block actual=" << actual_block_count << "  need={3|4}" << '\n'; }
            if( face_vec_format == 0 )
              face_vec_format = actual_block_count;
            else if( actual_block_count != face_vec_format )
              cerr_parse << "Not same face block format" << '\n';

            //
            if( actual_block_count == 4 )
              if( !parent.option.convert_quad_face_to_quad_triangle )
                have_quad_face = true;

          }
        }

      }
      std::cerr.flush();
    }

  }
}

