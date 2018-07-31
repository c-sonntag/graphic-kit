#pragma once

#include <graphic_toolkit/opengl/index_buffer.h>

namespace graphic_toolkit {
  namespace opengl {

    inline index_buffer::index_buffer() :
      buffer( raiigl::buffer_type::ElementArray ),
      buffer_length( 0 ), buffer_bytesize( 0 )
    {}

    inline void index_buffer::allocate()
    {
      //
      buffer_length = indices.size();
      buffer_bytesize = buffer_length * sizeof( indice_t );

      //
      buffer.bind_and_send( static_cast<GLsizeiptr>( buffer_bytesize ),  indices.data() );
    }

  }
}
