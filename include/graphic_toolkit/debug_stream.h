#pragma once

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>

#include <iostream>

#define OUTPUT_VAR(var) " " #var "(" << var << ") "
#define OV(var) OUTPUT_VAR(var)

inline std::ostream & operator<<( std::ostream & os, const QVector2D & v2 )
{
  os << v2.x() << "," << v2.y();
  return os;
}

inline std::ostream & operator<<( std::ostream & os, const QVector3D & v3 )
{
  os << v3.x() << "," << v3.y() << "," << v3.z();
  return os;
}

inline std::ostream & operator<<( std::ostream & os, const QPoint & p )
{
    os << p.x() << "," << p.y();
    return os;
}

