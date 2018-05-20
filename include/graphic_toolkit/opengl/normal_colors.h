#pragma once
#ifndef graphic_toolkit_opengl_normal_colors_h
#define graphic_toolkit_opengl_normal_colors_h

#include <QVector3D>

namespace graphic_toolkit {
  namespace opengl {
    namespace normal_colors {

      constexpr inline QVector3D normalize( uchar r, uchar g, uchar b ) { return QVector3D( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f ); }

      constexpr QVector3D black( 0.f, 0.f, 0.f );
      constexpr QVector3D white( 1.f, 1.f, 1.f );

      constexpr QVector3D intense_red( 1.f, 0.f, 0.f );
      constexpr QVector3D intense_green( 0.f, 1.f, 0.f );
      constexpr QVector3D intense_blue( 0.f, 0.f, 1.f );


      /** @see Hue on https://www.w3schools.com/colors/colors_picker.asp */

      constexpr QVector3D orange( normalize( 255, 191, 0 ) );
      constexpr QVector3D yellow( normalize( 255, 255, 0 ) );
      constexpr QVector3D nvidia_green_1( normalize( 153, 255, 51 ) );
      constexpr QVector3D nvidia_green_2( normalize( 204, 255, 51 ) );


    }
  }
}

#endif

