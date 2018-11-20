#pragma once

#include <bits/move.h>

#include <gtk/types.hpp>
#include <gtk/color/rgba.hpp>

#include <cmath>

namespace gtk {
  namespace color {


    template<typename TColor>
    inline TColor hsl_plot_color_format( float r, float g, float b )
    { return TColor( r, g, b );}

    template<>
    inline rgb hsl_plot_color_format<rgb>( float r, float g, float b )
    { return rgb( uchar( 255.f * b ), uchar( 255.f * g ), uchar( 255.f * r ) ); }

    // ---- ----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

    struct hsl
    {
     public:

      union
      {
        struct { float hue, saturation, lightness; };
        struct { float h, s, l; };
      };

     public:
      inline hsl() :
        hue( 0.1f ), saturation( 1.f ), lightness( 1.f )
      {}

      explicit constexpr inline hsl( const uint8_t _hue, const uint8_t _saturation, const uint8_t _lightness ) :
        hue( std::move( _hue ) ), saturation( std::move( _saturation ) ), lightness( std::move( _lightness ) )
      {}

     public:
      /**
       * @author adaptation from https://www.programmingalgorithms.com/algorithm/hsl-to-rgb?lang=C%2B%2B
       */
      inline float hsl_hue_to_rgb( float v1, float v2, float vH )
      {
        if( vH < 0.f ) vH += 1.f;
        if( vH > 1.f ) vH -= 1.f;
        if( ( 6.f * vH ) < 1.f ) return v1 + ( v2 - v1 ) * 6.f * vH;
        if( ( 2.f * vH ) < 1.f ) return v2;
        if( ( 3.f * vH ) < 2.f ) return v1 + ( v2 - v1 ) * ( ( 2.f / 3.f ) - vH ) * 6.f;
        return v1;
      }

      /**
       * @see adaptation from https://www.programmingalgorithms.com/algorithm/hsl-to-rgb?lang=C%2B%2B
       */
      template<typename TRgbColor>
      inline TRgbColor to_rgb( float hue = 0.f, float saturation = 1.f, float lightness = 1.f )
      {
        const float v2(
          ( lightness < 0.5f ) ?
          ( lightness * ( 1.f + saturation ) ) :
          ( ( lightness + saturation ) - ( lightness * saturation ) )
        );
        const float v1( 2.f * lightness - v2 );
        return hsl_plot_color_format<TRgbColor>(
          hsl_hue_to_rgb( v1, v2, hue + ( 1.0f / 3.f ) ),  // R
          hsl_hue_to_rgb( v1, v2, hue ),                   // G
          hsl_hue_to_rgb( v1, v2, hue - ( 1.0f / 3.f ) )   // B
        );
      }


    };

    #pragma GCC diagnostic pop






  }
}

