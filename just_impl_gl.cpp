#include <graphic_toolkit/opengl/primitives_heap.h>

#include <QVector2D>
#include <QVector3D>

#include <iostream>

namespace graphic_toolkit {

  void test()
  {

    using primitives_heap_t = opengl::primitives_heap<QVector3D, QVector3D, QVector2D>;
    primitives_heap_t heap(
      opengl::attrib_pointer( 0, 3, GL_FLOAT, true ),
      opengl::attrib_pointer( 1, 2, GL_FLOAT, true ),
      opengl::attrib_pointer( 2, 2, GL_FLOAT, true )
    );

    {
      primitives_heap_t::vertex_expander line_loop( heap.complete_primitive( opengl::primitive_type::line_loop ) );
      line_loop.push(
        QVector3D( 0.f, 0.f, 0.f ),
        QVector3D( 0.f, 0.f, 0.f ),
        QVector2D( 0.f, 0.f )
      );
    }



    heap.draw(
      *static_cast<QOpenGLFunctions_3_3_Core *>( nullptr ),
      *static_cast<QOpenGLShaderProgram *>( nullptr )
    );

  }

}
