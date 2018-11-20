#pragma once

#include <gtk/opengl/primitives_heap/vertex_buffer.hpp>

namespace gtk {
  namespace opengl {

    template<typename  ... TListTypes>
    inline vertex_buffer<TListTypes...>::vertex_buffer() :
      buffer( raiigl::buffer_type::Array ),
      buffer_length( 0 ), buffer_bytesize( 0 )
    {}

    template<typename  ... TListTypes>
    inline void vertex_buffer<TListTypes...>::allocate()
    {
      //
      buffer_length = rows.size();
      buffer_bytesize = buffer_length * sizeof( row_t );

      //
      buffer.bind_and_send( static_cast<GLsizeiptr>( buffer_bytesize ), rows.data() );
    }


  }
}

