#pragma once

#include <graphic_toolkit/opengl/vertex_buffer.h>

namespace graphic_toolkit {
  namespace opengl {

    template<typename  ... TListTypes>
    inline vertex_buffer<TListTypes...>::vertex_buffer() :
      buffer( QOpenGLBuffer::Type::VertexBuffer ),
      buffer_length( 0 ), buffer_bytesize( 0 )
    {}

    template<typename  ... TListTypes>
    inline void vertex_buffer<TListTypes...>::allocate()
    {
      //
      buffer_length = rows.size();
      buffer_bytesize = buffer_length * sizeof( row_t );

      //
      if ( !buffer.isCreated() )
        buffer.create();

      //
      buffer.bind();
      buffer.allocate( rows.data(), static_cast<int>( buffer_bytesize ) );
    }


  }
}

