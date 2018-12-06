#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <iomanip>

#define OUTPUT_VAR( var ) " " #var "(" << var << ") "
#define OV( var ) OUTPUT_VAR( var )

template<typename T>
inline std::ostream& operator <<( std::ostream& os, const glm::vec<2, T>& v2 )
{
  os << std::setprecision( 5 );
  os << std::setw( 5 ) << v2.x << "," << std::setw( 5 ) << v2.y;
  return os;
}

template<typename T>
inline std::ostream& operator <<( std::ostream& os, const glm::vec<3, T>& v3 )
{
  os << std::setprecision( 5 );
  os << std::setw( 5 ) << v3.x << "," << std::setw( 5 ) << v3.y << "," << std::setw( 5 ) << v3.z;
  return os;
}

#define debug_cout std::cout
