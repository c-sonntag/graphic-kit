#include <graphic_toolkit/opengl/quick_text_bff_wrapper.h>

#include <QOpenGLPixelTransferOptions>

#include <stdexcept>
#include <string.h>
#include <memory>

namespace graphic_toolkit {
  namespace opengl {


    // ---- ---- ---- ----

    constexpr uint max_size_mega     = 10;

    constexpr uint bfg_maxstring     = 255; /**< Maximum string length */
    constexpr uint width_data_offset =  20; /**< Offset to width data with BFF file          */
    constexpr uint map_data_offset   = 276; /**< Offset to texture image data with BFF file  */
    constexpr uint max_size          = max_size_mega * 1024 * 1024;

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
    quick_text_bbf_wrapper::bbf_font::bbf_font( QFile & in, QOpenGLTexture & texture )
    {
      //
      if ( !in.isOpen() || !in.isReadable() )
        throw std::runtime_error( ( "[quick_text_bbf_wrapper] need opened and readable file : " + in.fileName() ).toStdString() );

      // Get Filesize
      const qint64 is_size( in.size() );

      //
      if ( is_size > max_size )
        throw std::runtime_error(
          "[quick_text_bbf_wrapper] size("
          + std::to_string( is_size ) + ") is greater than max authorized size "
          + std::to_string( max_size_mega ) + "MB"
        );

      //
      const size_t size = size_t( is_size );

      // allocate space for file data
      uchar * const dat( new uchar[size] );

      //
      try
      {

        //
        const size_t read( size_t( in.read( reinterpret_cast<char *>( dat ), size ) ) );
        if ( read != size )
          throw std::runtime_error( "[quick_text_bbf_wrapper] read operation fail" );

        // Check ID is 'BFF2'
        if ( ( dat[0] != 0xBF ) || ( dat[1] != 0xF2 ) )
          throw std::runtime_error( "[quick_text_bbf_wrapper] need BFF v2" );


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
          throw std::runtime_error( "[quick_text_bbf_wrapper] filesize is incorrect" );

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
          throw std::runtime_error( "[quick_text_bbf_wrapper] unsupported BPP format" );
        }

        // Allocate space for image
        uchar * const img( new uchar[( width * height ) * ( bpp / 8 )] );

        //
        try
        {

          // Grab char widths8
          memcpy( characters_width, &dat[width_data_offset], 256 );

          // Grab image data
          memcpy( img, &dat[map_data_offset], ( width * height ) * ( bpp / 8 ) );

          // Create Texture
          texture.create();
          texture.bind();


          // Fonts should be rendered at native resolution so no need for texture filtering
          texture.setMinMagFilters(
            QOpenGLTexture::Filter::Nearest,
            QOpenGLTexture::Filter::Nearest
          );

          // Stop chararcters from bleeding over edges
          texture.setWrapMode( QOpenGLTexture::ClampToEdge );

          // Tex creation params are dependent on BPP
          const QOpenGLTexture::PixelFormat format(
            ( img_type == blend_flag::alpha ) ?
            QOpenGLTexture::PixelFormat::Red :
            ( ( img_type == blend_flag::rgb ) ?
              QOpenGLTexture::PixelFormat::RGB :
              QOpenGLTexture::PixelFormat::RGBA )
          );

          const QOpenGLTexture::TextureFormat tex_format(
            ( img_type == blend_flag::alpha ) ?
            QOpenGLTexture::TextureFormat::R8_UNorm :
            ( ( img_type == blend_flag::rgb ) ?
              QOpenGLTexture::TextureFormat::RGB8_UNorm :
              QOpenGLTexture::TextureFormat::RGBA8_UNorm )
          );

          //
          texture.setFormat( tex_format );
          texture.setSize( int( width ), int( height ) );
          texture.setMipLevels( 1 );
          texture.allocateStorage( format, QOpenGLTexture::UInt8 );

          //
          QOpenGLPixelTransferOptions uploadOptions;
          uploadOptions.setAlignment( 1 );

          //
          texture.setData( format, QOpenGLTexture::PixelType::UInt8, img, &uploadOptions );
        }
        catch ( ... )
        {
          delete [] img;
          throw; //with no operand rethrows the "current exception"
        }
      }
      catch ( ... )
      {
        delete [] dat;
        throw; //with no operand rethrows the "current exception"
      }

    }


    // ---- ---- ---- ----

    quick_text_bbf_wrapper::quick_text_bbf_wrapper( QFile & in ) :
      texture( QOpenGLTexture::Target::Target2D ),
      font( in, texture )
    { }

  }
}
