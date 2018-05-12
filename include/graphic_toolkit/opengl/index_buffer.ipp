#pragma once

#include <graphic_toolkit/opengl/index_buffer.h>

namespace graphic_toolkit {
  namespace opengl {

    inline index_buffer::index_buffer() :
      buffer( QOpenGLBuffer::Type::IndexBuffer ),
      buffer_length( 0 ), buffer_bytesize( 0 )
    {}

    inline void index_buffer::allocate()
    {

      //
      buffer_length = indices.size();
      buffer_bytesize = buffer_length * sizeof( indice_t );

      //
      if ( !buffer.isCreated() )
        buffer.create();

      //
      buffer.bind();
      buffer.allocate( indices.data(), static_cast<int>( buffer_bytesize ) );

    }

  }
}
