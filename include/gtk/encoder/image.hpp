#pragma once

#include <gtk/types.hpp>
#include <gtk/classes/non_copyable_movable.hpp>

#include <erc/file.h>
#include <erc/file_id.h>
#include <erc/inventory_package.h>

#include <memory>
#include <string>


namespace gtk {
  namespace encoder {

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
    struct image
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
      uchar* const m_data;

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

     public:
      gtk_classes_non_copyable_movable( image )

     public:
      inline const uchar* data() const { return m_data; }

     public:
      image vertical_flip();
    };

    // ---- ----

    __forceinline image image::load_from_local_erc( const erc::file_id& erc_id, const bool vertical_flip_it )
    {
      const erc::embedded_file& erc( erc::inventory_package::get_local_first_embedded_file( erc_id, "gtk::encoder::image::load_from_local_erc" ) );
      image img( load_from_erc( erc, vertical_flip_it ) );
      erc.unallocate_proper_data();
      return img;
    }

  }
}
