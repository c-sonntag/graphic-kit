#include <gk/opengl/quick_text_bff_wrapper.hpp>

#include <gk/opengl/texture.hpp>

#include <stdexcept>
#include <string.h>
#include <memory>

namespace gk {
  namespace opengl {


    // ---- ---- ---- ----

    constexpr uint max_size_mega     = 10;

    constexpr uint bfg_maxstring     = 255; /**< Maximum string length */
    constexpr uint width_data_offset =  20; /**< Offset to width data with BFF file          */
    constexpr uint map_data_offset   = 276; /**< Offset to texture image data with BFF file  */
    constexpr uint max_size          = max_size_mega * 1024 * 1024;
    constexpr uint min_size          = map_data_offset;

    // ---- ---- ---- ----

    enum class blend_flag : uint
    {
      none  = 0x0,
      alpha = 0x1,
      rgb   = 0x2,
      rgba  = 0x4,
    };

    // ---- ----

    /**
     * @see bff tools : http://www.codehead.co.uk/cbfg/
     * @see bff reader exemple : https://github.com/CodeheadUK/CBFG/tree/master/examples/BFF%20Example
     */
    quick_text_bbf_wrapper::bbf_font::bbf_font( const std::string & bbf_data_font, raiigl::texture & texture )
    {
      //
      const size_t size( bbf_data_font.size() );
      if ( size > max_size )
        throw std::runtime_error(
          "[gk::opengl::quick_text_bbf_wrapper] size("
          + std::to_string( size ) + ") is greater than max authorized size "
          + std::to_string( max_size_mega ) + "MB"
        );
      else if ( size < min_size )
        throw std::runtime_error(
          "[gk::opengl::quick_text_bbf_wrapper] size("
          + std::to_string( size ) + ") is smaller than min authorized size "
          + std::to_string( min_size ) + " bytes"
        );

      //
      const uchar * const dat( reinterpret_cast<const uchar *>( bbf_data_font.data() ) );


      // Check ID is 'BFF2'
      if ( ( dat[0] != 0xBF ) || ( dat[1] != 0xF2 ) )
        throw std::runtime_error( "[gk::opengl::quick_text_bbf_wrapper] Need BFF v2" );


      // Grab the rest of the header
      memcpy( &width, &dat[2], sizeof( uint ) );
      memcpy( &height, &dat[6], sizeof( uint ) );
      memcpy( &cell_width, &dat[10], sizeof( uint ) );
      memcpy( &cell_height, &dat[14], sizeof( uint ) );

      const uchar bpp = dat[18];
      start_char = dat[19];
      end_char = 256;

      // Check filesize
      if ( size != ( map_data_offset  + ( ( width * height ) * ( bpp / 8 ) ) ) )
        throw std::runtime_error( "[gk::opengl::quick_text_bbf_wrapper] Filesize is incorrect : (map_data_offset+width*height*bpp/8)" );

      // Calculate font params
      row_pitch = width / cell_width;
      col_factor = float( cell_width ) / float( width );
      row_factor = float( cell_height ) / float( height );
      y_offset = cell_height;

      //
      normal_scale = glm::vec2(
                       1.f / float( cell_width ),
                       1.f / float( cell_height )
                     );

      blend_flag img_type( blend_flag::none );

      // Determine blending options based on BPP
      switch ( bpp )
      {
      case 8: // Greyscale
        img_type = blend_flag::alpha;
        break;

      case 24: // RGB
        img_type = blend_flag::rgb;
        break;

      case 32: // RGBA
        img_type = blend_flag::rgba;
        break;

      default: // Unsupported BPP
        throw std::runtime_error( "[gk::opengl::quick_text_bbf_wrapper] unsupported BPP format" );
      }

      // Grab char widths8
      memcpy( characters_width, &dat[width_data_offset], 256 );

      // Grab image data
      const uchar * const img( &dat[map_data_offset] );

      // Create Texture
      texture.bind();

      // Fonts should be rendered at native resolution so no need for texture filtering
      texture.set_param_minifying_and_magnification_filter(
        raiigl::minifying_filter_type::Nearest,
        raiigl::magnification_filter_type::Nearest
      );

      // Stop chararcters from bleeding over edges
      texture.set_param_wrap( raiigl::wrap_type::ClampToEdge );

      // Tex creation params are dependent on BPP
      const raiigl::pixel_format format(
        ( img_type == blend_flag::alpha ) ?
        raiigl::pixel_format::Red :
        ( ( img_type == blend_flag::rgb ) ?
          raiigl::pixel_format::RGB :
          raiigl::pixel_format::RGBA )
      );

      //
      const raiigl::internal_format_type internal_format(
        ( img_type == blend_flag::alpha ) ?
        raiigl::internal_format_type::R8 :
        ( ( img_type == blend_flag::rgb ) ?
          raiigl::internal_format_type::RGB8 :
          raiigl::internal_format_type::RGBA8 )
      );


      //
      texture.send_image2d(
        0,
        internal_format,
        width, height,
        format, raiigl::pixel_type::UnsignedByte,
        img
      );

    }

    // ---- ---- ---- ----

    quick_text_bbf_wrapper::quick_text_bbf_wrapper( const std::string & bbf_data_font ) :
      texture( raiigl::texture_type::Texture2D ),
      font( bbf_data_font, texture )
    { }

  }
}
