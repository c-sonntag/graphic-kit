#pragma once

#include <graphic_toolkit/types.h>

#include <bits/move.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace graphic_toolkit {
  namespace color {

    struct rgb
    {
      union
      {
        struct {uint8_t red, green, blue; };
        struct {uint8_t r, g, b; };
      };

      inline rgb() : red( 0 ), green( 0 ), blue( 0 ) {}

      explicit constexpr inline rgb( const uint8_t _red, const uint8_t _green, const uint8_t _blue ) : red( std::move( _red ) ), green( std::move( _green ) ), blue( std::move( _blue ) ) {}
      constexpr inline rgb( int _red, int _green, int _blue ) : red( std::move( _red ) ), green( std::move( _green ) ), blue( std::move( _blue ) ) {}

      constexpr inline rgb( const uint32_t hex ) : red( ( hex >> 16 ) & 255 ), green( ( hex >> 8 ) & 255 ), blue( hex & 255 ) {}

      explicit constexpr inline rgb( const float normal_red, const float normal_green, const float normal_blue ) : red( uint8_t( normal_red * 255.f ) ), green( uint8_t( normal_green * 255.f ) ), blue( uint8_t( normal_blue * 255.f ) ) {}
      explicit constexpr inline rgb( const glm::vec3 normal_rgb ) : rgb( normal_rgb.r, normal_rgb.g, normal_rgb.b ) {}

      constexpr inline glm::vec3 normalize() const { return glm::vec3( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f ); }

      constexpr inline operator glm::vec3() const { return normalize(); }
    };

    // ---- ----

    struct rgba
    {
      union
      {
        struct {uint8_t red, green, blue, alpha; };
        struct {uint8_t r, g, b, a; };
      };

      inline rgba() : red( 0 ), green( 0 ), blue( 0 ), alpha( 255 ) {}

      explicit constexpr inline rgba( const uint8_t _red, const uint8_t _green, const uint8_t _blue, const uint8_t _alpha = 255 ) : red( std::move( _red ) ), green( std::move( _green ) ), blue( std::move( _blue ) ), alpha( std::move( _alpha ) ) {}
      explicit constexpr inline rgba( const int _red, const int _green, const int _blue, const int _alpha = 255 ) : red( std::move( _red ) ), green( std::move( _green ) ), blue( std::move( _blue ) ), alpha( std::move( _alpha ) ) {}
      explicit constexpr inline rgba( const rgb rgb, const uint8_t _alpha = 255 ): red( std::move( rgb.red ) ), green( std::move( rgb.green ) ), blue( std::move( rgb.blue ) ), alpha( std::move( _alpha ) ) {}

      constexpr inline rgba( const uint32_t hex, uint8_t _alpha = 255 ) : red( ( hex >> 16 ) & 255 ), green( ( hex >> 8 ) & 255 ), blue( hex & 255 ), alpha( std::move( _alpha ) ) {}

      explicit constexpr inline rgba( const float normal_red, const float normal_green, const float normal_blue, const float normal_alpha ) : red( uint8_t( normal_red * 255.f ) ), green( uint8_t( normal_green * 255.f ) ), blue( uint8_t( normal_blue * 255.f ) ), alpha( uint8_t( normal_alpha * 255.f ) ) {}
      explicit constexpr inline rgba( const glm::vec4 normal_rgba ) : rgba( normal_rgba.r, normal_rgba.g, normal_rgba.b, normal_rgba.a ) {}
      explicit constexpr inline rgba( const glm::vec3 normal_rgb ) : rgba( normal_rgb.r, normal_rgb.g, normal_rgb.b, 1.f ) {}

      inline glm::vec4 normalize() const { return glm::vec4( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f, float( a ) / 255.f ); }

      inline operator glm::vec4() const { return normalize(); }
    };


    // ---- ---- ---- ----

    constexpr inline glm::vec3 normalize( const uint8_t r, const uint8_t g, const uint8_t b )
    { return glm::vec3( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f ); }

    inline glm::vec4 normalize( const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a )
    { return glm::vec4( float( r ) / 255.f, float( g ) / 255.f, float( b ) / 255.f, float( a ) / 255.f ); }


  }
}

