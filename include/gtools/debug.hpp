#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <iomanip>

#define OUTPUT_VAR( var ) " " #var "(" << var << ") "
#define OV( var ) OUTPUT_VAR( var )

inline std::ostream& operator <<( std::ostream& os, const glm::vec2& v2 )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << v2.x<< "," << std::setw( 5 ) << v2.y;
  return os;
}

inline std::ostream& operator <<( std::ostream& os, const glm::vec3& v3 )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << v3.x << "," << std::setw( 5 ) << v3.y << "," << std::setw( 5 ) << v3.z;
  return os;
}

