#pragma once

#include <gk/types.hpp>
#include <gk/classes/non_copyable.hpp>

#include <erc/file.h>
#include <erc/file_id.h>
#include <erc/inventory_package.h>

#include <memory>
#include <string>


namespace gk {
  namespace decoder {

    //enum class image_format
    //{
    //  Unknown = 0,
    //  JPG, /**< JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib) */
    //  PNG, /**< PNG 1/2/4/8/16-bit-per-channel                                                       */
    //  TGA, /**< TGA (not sure what subset, if a subset)                                              */
    //  BMP, /**< BMP non-1bpp, non-RLE                                                                */
    //  GIF  /**< GIF (*comp always reports as 4-channel)                                              */
    //};

    /** @todo import DDS file for compressed data */

    /**
     * @see For more detail of the implementation : https://github.com/nothings/stb/blob/master/stb_image.h
     * @note Great thanks for the stb_image library header implementation.
     * @note Information about capability :
     *    JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
     *    PNG 1/2/4/8/16-bit-per-channel
     *    TGA (not sure what subset, if a subset)
     *    BMP non-1bpp, non-RLE
     *    GIF (*comp always reports as 4-channel)
     */
    struct image : public gk::classes::non_copyable
    {
     public:
      static image load_from_file( const std::string& file_path, const bool vertical_flip_it = false );
      static image load_from_memory( const std::string& input_data, const bool vertical_flip_it = false );
      static image load_from_erc( const erc::embedded_file& erc, const bool vertical_flip_it = false );

      static __forceinline image load_from_local_erc( const erc::file_id& erc_id, const bool vertical_flip_it = false );

     public:
      //const image_format format;
      const uint width, height;
      const ushort channels;
      const bool is_vertical_flip;
      const size_t size;

     private:
      uchar* m_data;

     public:
      inline image( const uint _width, const uint _height, const ushort _channels, const bool _is_vertical_flip, uchar* const _data ) :
        width( std::move( _width ) ), height( std::move( _height ) ),
        channels( std::move( _channels ) ),
        is_vertical_flip( std::move( _is_vertical_flip ) ),
        size( width * height * channels ),
        m_data( std::move( _data ) )
      {}

     public:
      ~image();

     protected:
      bool invalid_state = false;

     public:
      inline image( image&& i ) :
        width( std::move( i.width ) ), height( std::move( i.height ) ),
        channels( std::move( i.channels ) ),
        is_vertical_flip( std::move( i.is_vertical_flip ) ),
        size( std::move( i.size ) ),
        m_data( std::move( i.m_data ) ),
        invalid_state( std::move( i.invalid_state ) )
      { i.invalid_state = true; }

     public:
      inline const uchar* data() const { return m_data; }

     public:
      image vertical_flip();
    };

    // ---- ----

    __forceinline image image::load_from_local_erc( const erc::file_id& erc_id, const bool vertical_flip_it )
    {
      const erc::embedded_file& erc( erc::inventory_package::get_local_first_embedded_file( erc_id, "gk::decoder::image::load_from_local_erc" ) );
      image img( load_from_erc( erc, vertical_flip_it ) );
      erc.unallocate_proper_data();
      return img;
    }

  }
}
