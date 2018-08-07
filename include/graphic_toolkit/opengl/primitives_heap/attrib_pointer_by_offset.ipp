#pragma once

#include <graphic_toolkit/opengl/primitives_heap/attrib_pointer_by_offset.h>
#include <graphic_toolkit/opengl/primitives_heap/vertex_buffer.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {

    template<typename T>
    inline attrib_pointer_by_offset::attrib_pointer_by_offset( attrib_pointer va, size_t _offset, size_t row_size, T * ) :
      attrib_pointer( std::move( va ) ),
      ctype( typeid( T ) ),
      bytesize( sizeof( T ) ),
      offset( std::move( _offset ) ),
      opposed_offset( row_size - offset - size_t( bytesize ) )
    {
      if ( ( opposed_offset > row_size ) || ( offset > row_size ) )
        throw std::runtime_error( "offset or opposed_offset can't be highter than row_size" );
    }

    // ---- ---- ---- ----

template< typename  ... TListTypes >
template<typename TAttribPointerTypeLast>
inline void attribs_pointers_by_offset<TListTypes...>::insert_attributes( size_t offset, attrib_pointer & va )
{
  using row_type = typename vertex_buffer<TListTypes...>::row_t;

  emplace_back(
    va,
    offset,
    sizeof( row_type ),
    static_cast<TAttribPointerTypeLast *>( nullptr )
  );
}

template< typename  ... TListTypes >
template<typename TAttribPointerTypeCurrent, typename ...TAttribPointerTypeNext, typename... Args>
inline void attribs_pointers_by_offset<TListTypes...>::insert_attributes( size_t offset, attrib_pointer & va, Args & ... va_next )
{
  using row_type = typename vertex_buffer<TListTypes...>::row_t;

  emplace_back(
    va,
    offset,
    sizeof( row_type ),
    static_cast<TAttribPointerTypeCurrent *>( nullptr )
  );

  insert_attributes<TAttribPointerTypeNext...>(
    offset + sizeof( TAttribPointerTypeCurrent ),
    va_next...
  );
}

template< typename  ... TListTypes >
template< typename... Args >
inline  attribs_pointers_by_offset<TListTypes...>::attribs_pointers_by_offset( Args ... attributes )
{
  static_assert(
    sizeof...( TListTypes ) == sizeof...( Args ),
    "You need to set the attribs_pointers_by_offset, with correct number of type and atttributes definitions."
    "Exemple for drawer : "
    "opengl::drawer<QVector3D, GLfloat> obd("
    "  opengl::attrib_pointer( 0, 3, GL_FLOAT, true ),"
    "  opengl::attrib_pointer( 1, 1, GL_FLOAT, true )"
    ");"
  );
  //
  reserve( sizeof...( TListTypes ) );
  insert_attributes<TListTypes...>( 0, attributes... );
}



  }
}

