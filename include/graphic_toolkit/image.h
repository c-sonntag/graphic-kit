#pragma once
#ifndef graphic_toolkit_image_h
#define graphic_toolkit_image_h



#include <graphic_toolkit/types.h>
#include <graphic_toolkit/classes/non_copyable_movable.hpp>

#include <erc/file.h>

#include <memory>
#include <string>


namespace graphic_toolkit {

  //enum class image_format
  //{
  //  Unknown = 0,
  //  JPG, /**< JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib) */
  //  PNG, /**< PNG 1/2/4/8/16-bit-per-channel                                                       */
  //  TGA, /**< TGA (not sure what subset, if a subset)                                              */
  //  BMP, /**< BMP non-1bpp, non-RLE                                                                */
  //  GIF  /**< GIF (*comp always reports as 4-channel)                                              */
  //};

  /**
   * @see For more detail of the implementation : https://github.com/nothings/stb/blob/master/stb_image.h
   * @note Great thanks for the stb_image libraty header implementation.
   * @note Information about capability :
   *  JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
   *  PNG 1/2/4/8/16-bit-per-channel
   *  TGA (not sure what subset, if a subset)
   *  BMP non-1bpp, non-RLE
   *  GIF (*comp always reports as 4-channel)
   */
  struct image
  {
   public:
    static std::unique_ptr<image> load_from_memory( const std::string & input_data, const bool vertical_flip_it = false );
    static std::unique_ptr<image> load_from_file( const std::string & file_path, const bool vertical_flip_it = false );
    static std::unique_ptr<image> load_from_erc( const erc::embedded_file & erc, const bool vertical_flip_it = false );

   public:
    //const image_format format;
    const uint width, height;
    const ushort channels;
    const bool is_vertical_flip;
    const size_t size;

   private:
    uchar * const m_data;

   public:
    inline image( const uint _width, const uint _height, const ushort _channels, const bool _is_vertical_flip, uchar * const _data ) :
      width( std::move( _width ) ), height( std::move( _height ) ),
      channels( std::move( _channels ) ),
      is_vertical_flip( std::move( _is_vertical_flip ) ),
      size( width * height * channels ),
      m_data( std::move( _data ) )
    {}

   public:
    ~image();

   public:
    graphic_toolkit_classes_non_copyable_movable( image )

   public:
    inline const uchar * data() const { return m_data; }

   public:
    image vertical_flip();
  };


}



#endif
