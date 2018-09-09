#pragma once

#include <graphic_toolkit/image.h>
#include <graphic_toolkit/base_format.h>

#include <raiigl/texture.hpp>

#include <stdexcept>
#include <sstream>

namespace graphic_toolkit {
  namespace opengl {

    inline raiigl::texture texture_from_image( const image & img )
    {

      //
      const bool good_image_format(
        graphic_toolkit::base_format::is_pow_of( 2u, img.width ) &&
        graphic_toolkit::base_format::is_pow_of( 2u, img.height )
      );
      if ( !good_image_format )
      {
        std::ostringstream oss;
        oss << "[graphic_toolkit::opengl::texture_from_image] Incompatible image dimension ("
            << img.width << "x" << img.height << ") need power of 2 like 128x128, 256x128 etc..";
        throw std::runtime_error( oss.str() );
      }

      //
      raiigl::internal_format_type internal_format_type;
      raiigl::pixel_format pixel_format;
      if ( img.channels == 3 )
      {
        internal_format_type = raiigl::internal_format_type::RGB;
        pixel_format = raiigl::pixel_format::RGB;
      }
      else if ( img.channels == 4 )
      {
        internal_format_type = raiigl::internal_format_type::RGBA;
        pixel_format = raiigl::pixel_format::RGBA;
      }
      else
        throw std::runtime_error(
          "[graphic_toolkit::opengl::texture_from_image] Unknown channels("
          + std::to_string( img.channels ) + ") of this image"
        );

      //
      raiigl::texture tex( raiigl::texture_type::Texture2D );
      tex.bind();
      tex.send_image2d(
        0,
        internal_format_type,
        img.width, img.height,
        pixel_format,
        raiigl::pixel_type::UnsignedByte,
        img.data()
      );
      tex.set_param_minifying_and_magnification_filter(
        raiigl::minifying_filter_type::Nearest,
        raiigl::magnification_filter_type::Nearest
      );
      tex.unbind();

      //
      return tex;
    }

  }
}