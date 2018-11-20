#pragma once

#include <gtk/color/hsl.hpp>

#include <vector>

namespace gtk {
  namespace color {

  template<typename TColor>
  inline std::vector<TColor> hsl_plot( uint size, float hsl_over = 0.f, float hsl_start = 0.f, float hsl_end = 360.f )
  {
    static const float hsl_saturation( 1.f ), hsl_lightness( 0.7f );

    //
    std::vector<TColor> plot( size );
    const float hue_padding( ( hsl_end - hsl_start ) / float(size) );

    //
    for( uint i( 0 ); i < size; ++i )
    {
      const float decal( ( hsl_over / 1000.f ) * ( 360.f / 10.f ) ); // 10 color between 0->10000
      const float mult( hsl_over / 1000.f );                         // Maybe other effec by "hover"
      const float hue( std::fmod( decal + ( float(i) * hue_padding * mult ) / 360.f, 1.f ) );
      plot[i] = hsl(hue, hsl_saturation, hsl_lightness);
    }
    return plot;
  }

  }
}

