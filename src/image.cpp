#include <gtk/image.hpp>

#include <stdexcept>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION 1
#define STBI_ONLY_JPEG           1
#define STBI_ONLY_PNG            1
#define STBI_ONLY_BMP            1
#define STBI_ONLY_TGA            1
#define STBI_ONLY_GIF            1
#include <stb_image.h>

namespace gtk {

  static image internal_make_image( const int width, const int height, const int channels, const bool vertical_flip_it, uchar * const data )
  {
    if ( data == nullptr )
      throw std::runtime_error( "Error : " + std::string( stbi_failure_reason() ) );

    //
    if ( ( width <= 0 ) || ( height <= 0 ) || ( channels <= 0 ) )
    {
      stbi_image_free( data );
      throw std::runtime_error(
        "Error : Can't get image information width(" + std::to_string( width ) + ") height(" + std::to_string( height ) + ") channels(" + std::to_string( channels ) + ")"
      );
    }

    //
    return image(
             static_cast<uint>( width ), static_cast<uint>( height ),
             static_cast<ushort>( channels ),
             vertical_flip_it,
             data
           );
  }

  // ---- ----

  image image::load_from_file( const std::string & file_path, const bool vertical_flip_it )
  {
    stbi_set_flip_vertically_on_load( vertical_flip_it );

    //
    int width( 0 ), height( 0 ), channels( 0 );
    uchar * const data( stbi_load( file_path.c_str(), &width, &height, &channels, 0 ) );

    //
    try { return internal_make_image( width, height, channels, vertical_flip_it, data ); }
    catch ( const std::exception & e )
    { throw std::runtime_error( "[gtk::image::load_from_file] " + std::string( e.what() ) ); }
  }

  // ---- ----

  static image internal_make_image_from_memory( const std::string & input_data, const bool vertical_flip_it )
  {
    stbi_set_flip_vertically_on_load( vertical_flip_it );

    //
    int width( 0 ), height( 0 ), channels( 0 );
    uchar * const data( stbi_load_from_memory( reinterpret_cast<const uchar *>( input_data.data() ), static_cast<int>( input_data.size() ), &width, &height, &channels, 0 ) );

    //
    return internal_make_image( width, height, channels, vertical_flip_it, data );
  }

  image image::load_from_memory( const std::string & input_data, const bool vertical_flip_it )
  {
    try { return internal_make_image_from_memory( input_data, vertical_flip_it ); }
    catch ( const std::exception & e )
    { throw std::runtime_error( "[gtk::image::load_from_memory] " + std::string( e.what() ) ); }
  }

  image image::load_from_erc( const erc::embedded_file & erc, const bool vertical_flip_it )
  {
    const std::string input_data( erc.get_proper_data() );
    try { return internal_make_image_from_memory( input_data, vertical_flip_it ); }
    catch ( const std::exception & e )
    { throw std::runtime_error( "[gtk::image::load_from_erc] (file:" + erc.path + ") " + std::string( e.what() ) ); }
  }

  // ---- ---- ---- ----

  image::~image()
  {
    stbi_image_free( m_data );
  }

  // ---- ---- ---- ----

  image image::vertical_flip()
  {
    uchar * vertical_flip_data = new uchar[size];
    stbi__vertical_flip( vertical_flip_data, static_cast<int>( width ), static_cast<int>( height ), static_cast<int>( channels ) );
    return image( width, height, channels, !is_vertical_flip, vertical_flip_data );
  }


}

