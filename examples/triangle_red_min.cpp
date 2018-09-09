#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/vertex_array.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/gl330.hpp>

#include <graphic_toolkit/opengl/quick_program.h>

#include <erc/package_id.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

#include <memory>
#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


raiigl::gl330 gl;

struct EasyTrianglePainter : public AbstractPainter
{
 private:
  const raiigl::program program
  {
    graphic_toolkit::opengl::quick_program::open_from_sources(
      "#version 330 core\n" \
      "layout( location = 0 ) in vec3 vertex_from_buffer;\n" \
      "uniform mat4 MVP;\n" \
      "void main()\n" \
      "{ vec4 v = vec4( vertex_from_buffer, 1 ); gl_Position = MVP * v; };",
      /// ---
      "#version 330 core\n" \
      "out vec3 color;\n" \
      "uniform vec3 uniform_color;\n" \
      "void main()\n" \
      "{ color = uniform_color; };"
    )
  };

 private:
  raiigl::vertex_array vao;
  raiigl::buffer triangle_buffer{raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw};

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };
  const raiigl::uniform_variable uniform_color{ program, "uniform_color" };

 private:
  ModelViewProjection mvp;

 public:
  EasyTrianglePainter() {
    static const GLfloat g_vertex_buffer_data[]
    { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    program.use();
    vao.bind();
    triangle_buffer.bind_and_send( sizeof( g_vertex_buffer_data ), g_vertex_buffer_data );
    vao.attrib( triangle_buffer, 0, 3, raiigl::data_type::Float );
    vao.unbind();
    program.unuse();
  }

  float color_curve_count = 0.f;
  glm::vec3 color { 1.0f, 0.0f, 0.2f };

  void paint( GLFWwindow * ) {
    program.use();
    uniform_vertex_mvp.set( mvp.mvpRefresh() );

    color_curve_count += 0.1f;
    const float curve( std::sin( color_curve_count ) * 0.333f + 0.666f );
    uniform_color.set( color * curve );

    vao.bind();
    gl.draw_arrays( raiigl::primitive_type::Triangles, 0, 3 );
    vao.unbind();

    program.unuse();
  }

};


int main()
{

  try
  {
    //
    Window win( 800, 600, "Draw Triangle GLFW Windows" );

    //
    win.painters.push_back( std::make_unique<EasyTrianglePainter>() );

    //
    win.handleClose.push_back( []( GLFWwindow * window )
    {
      return glfwWindowShouldClose( window )
             || ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS );
    } );

    //
    win.run();

  }
  catch ( std::exception & e )
  {
    std::cerr << "Error : " <<  e.what() << std::endl;
    return -1;
  }

  return 0;
}

