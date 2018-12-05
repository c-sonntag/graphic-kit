#pragma once

#include <gk/decoder/image.hpp>
#include <gk/math.hpp>

#include <raiigl/program.hpp>
#include <raiigl/texture.hpp>

#include <stdexcept>
#include <sstream>

namespace gk {
  namespace opengl {

    inline raiigl::texture texture_from_image( const decoder::image& img )
    {
      //
      const bool good_image_format(
        gk::math::is_pow_of( 2u, img.width ) &&
        gk::math::is_pow_of( 2u, img.height )
      );
      if( !good_image_format )
      {
        std::ostringstream oss;
        oss << "[gk::opengl::texture_from_image] Incompatible image dimension ("
            << img.width << "x" << img.height << ") need power of 2 like 128x128, 256x128 etc..";
        throw std::runtime_error( oss.str() );
      }

      //
      raiigl::internal_format_type internal_format_type;
      raiigl::pixel_format pixel_format;
      if( img.channels == 3 )
      {
        internal_format_type = raiigl::internal_format_type::RGB;
        pixel_format = raiigl::pixel_format::RGB;
      }
      else if( img.channels == 4 )
      {
        internal_format_type = raiigl::internal_format_type::RGBA;
        pixel_format = raiigl::pixel_format::RGBA;
      }
      else
        throw std::runtime_error(
          "[gk::opengl::texture_from_image] Unknown channels("
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
      tex.set_param_wrap_str();
      tex.unbind();

      //
      return tex;
    }

    struct texture_binded : public raiigl::texture
    {
     public:
      const raiigl::textures_num texture_num;

     protected:
      bool invalid_state = false;

     public:
      inline texture_binded( raiigl::texture&& _texture ) :
        texture( std::move( _texture ) ),
        texture_num( raiigl::program::new_texture_index() )
      { bind_on_texture( texture_num ); unbind(); }

     public:
      inline texture_binded( const decoder::image& img ) :
        texture( texture_from_image( img ) ),
        texture_num( raiigl::program::new_texture_index() )
      { bind_on_texture( texture_num ); unbind(); }

     public:
      inline ~texture_binded() override
      {
        if( ( texture_num != raiigl::textures_num::None ) && !invalid_state )
          raiigl::program::free_texture_index( texture_num );
        invalid_state = true;
      }

     public:
      texture_binded( texture_binded& _t ) = default;
      texture_binded( const texture_binded& _t ) = default;

     public:
      inline texture_binded( texture_binded&& t ) :
        texture( std::move( dynamic_cast<raiigl::texture&&>( t ) ) ),
        texture_num( std::move( t.texture_num ) ),
        invalid_state( std::move( t.invalid_state ) )
      { const_cast<raiigl::textures_num&>( t.texture_num ) = raiigl::textures_num::None; t.invalid_state = true; }


    };

  }
}
