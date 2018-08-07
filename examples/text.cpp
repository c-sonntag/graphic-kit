#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/quick_text.h>
#include <graphic_toolkit/opengl/quick_text_expander.h>
#include <graphic_toolkit/opengl/normal_colors.h>

#include <erc/package_id.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

#include <memory>
#include <algorithm>
#include <iostream>
#include <chrono>


#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

static const erc::package_id shader_erc_id( "shaders" );

struct TextPainter : public AbstractPainter
{
 private:
  const raiigl::program program
  {
    graphic_toolkit::opengl::quick_program::open_from_local_erc(
      shader_erc_id.from( "shader.vert" ),
      shader_erc_id.from( "shader.frag" )
    )
  };

  graphic_toolkit::opengl::quick_text calibri{graphic_toolkit::opengl::quick_text_fonts::CalibriLight_1024};

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };

 private:
  GLuint vertex_buffer;
  GLuint vertex_array_id;

 private:
  ModelViewProjection mvp;

 private:
  std::chrono::system_clock::time_point start;

  const raiigl::gl330 gl;

 public:
  TextPainter() {

    start = std::chrono::system_clock::now();

    {
      //
      graphic_toolkit::opengl::text_expander yop( calibri.complete_text( "Yop ! Yap !" ) );
      yop.color = graphic_toolkit::opengl::normal_colors::intense_green;
      yop.align_h = graphic_toolkit::opengl::text_expander::center;
      yop.normal_size = 1.f;
    }

  }


  void paint( GLFWwindow * window ) {

    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_vertex_mvp.set( mvp.mvpRefresh() );

    //
    calibri.draw( gl, mvp.mvp() );
  }

};


int main()
{

  try
  {
    //
    Window win( 800, 600, "Draw Triangle GLFW Windows" );

    //
    win.painters.push_back( std::make_unique<TextPainter>() );

    //
    win.handleClose.push_back( []( GLFWwindow * window )
    {
      return glfwWindowShouldClose( window )
             || ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS );
    } );

    //
    win.run();

  }
  catch ( const std::exception & e )
  {
    std::cerr << "Error : " <<  e.what() << std::endl;
    return 1;
  }

  return 0;
}

