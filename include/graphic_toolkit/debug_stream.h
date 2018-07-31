#pragma once

#include <glm/vec2.hpp>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>

#include <iostream>
#include <iomanip>

#define OUTPUT_VAR(var) " " #var "(" << var << ") "
#define OV(var) OUTPUT_VAR(var)

inline std::ostream & operator<<( std::ostream & os, const glm::vec2 & v2 )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << v2.x() << "," << std::setw( 5 ) << v2.y();
  return os;
}

inline std::ostream & operator<<( std::ostream & os, const QSize & size )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << size.width() << "," << std::setw( 5 ) << size.height();
  return os;
}

inline std::ostream & operator<<( std::ostream & os, const QVector3D & v3 )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << v3.x() << "," << std::setw( 5 ) << v3.y() << "," << std::setw( 5 ) << v3.z();
  return os;
}

inline std::ostream & operator<<( std::ostream & os, const QPoint & p )
{
  os << std::setprecision( 2 );
  os << std::setw( 5 ) << p.x() << "," << std::setw( 5 ) << p.y();
  return os;
}

