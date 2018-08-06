#include <graphic_toolkit/image.h>

#include <stdexcept>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION 1
#define STBI_ONLY_JPEG           1
#define STBI_ONLY_PNG            1
#define STBI_ONLY_BMP            1
#define STBI_ONLY_TGA            1
#define STBI_ONLY_GIF            1
#include <stb_image.h>

namespace graphic_toolkit {

  static std::unique_ptr<image> internal_make_image( const std::string & err_group, const int width, const int height, const int channels, const bool vertical_flip_it, uchar * const data )
  {
    if ( data == nullptr )
      throw std::runtime_error( "[graphic_toolkit::image::" + err_group + "] Error : " + std::string( stbi_failure_reason() ) );

    //
    if ( ( width <= 0 ) || ( height <= 0 ) || ( channels <= 0 ) )
    {
      stbi_image_free( data );
      throw std::runtime_error(
        "[graphic_toolkit::image::" + err_group + "] Error : Can't get image information"
        "width(" + std::to_string( width ) + ") height(" + std::to_string( height ) + ") channels(" + std::to_string( channels ) + ")"
      );
    }

    //
    return std::make_unique<image>(
             static_cast<uint>( width ), static_cast<uint>( height ),
             static_cast<ushort>( channels ),
             vertical_flip_it,
             data
           );
  }

  // ---- ----

  std::unique_ptr<image> image::load_from_memory( const std::string & input_data, const bool vertical_flip_it )
  {
    stbi_set_flip_vertically_on_load( vertical_flip_it );

    //
    int width( 0 ), height( 0 ), channels( 0 );
    uchar * const data( stbi_load_from_memory( reinterpret_cast<const uchar *>( input_data.data() ), input_data.size(), &width, &height, &channels, 0 ) );

    //
    return internal_make_image( "load_from_memory", width, height, channels, vertical_flip_it, data );
  }

  std::unique_ptr<image> image::load_from_file( const std::string & file_path, const bool vertical_flip_it )
  {
    stbi_set_flip_vertically_on_load( vertical_flip_it );

    //
    int width( 0 ), height( 0 ), channels( 0 );
    uchar * const data( stbi_load( file_path.c_str(), &width, &height, &channels, 0 ) );

    //
    return internal_make_image( "load_from_file", width, height, channels, vertical_flip_it, data );
  }

  std::unique_ptr<image> image::load_from_erc( const erc::embedded_file & erc, const bool vertical_flip_it )
  { return load_from_memory( erc.get_proper_data(), vertical_flip_it ); }

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

