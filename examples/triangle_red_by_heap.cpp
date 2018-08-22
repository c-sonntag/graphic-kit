#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/gl330.hpp>

#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/primitives_heap.h>

#include <erc/package_id.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

#include <memory>
#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

static const erc::package_id shader_erc_id( "shaders" );

struct EasyTriangleHeapPainter : public AbstractPainter
{
 private:
  const raiigl::gl330 gl330;
  raiigl::program program
  {
    graphic_toolkit::opengl::quick_program::open_from_local_erc(
      shader_erc_id.from( "shader.vert" ),
      shader_erc_id.from( "shader.frag" )
    )
  };

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };
  const raiigl::uniform_variable uniform_color{ program, "uniform_color" };

 private:
  using heap_vertices_t = graphic_toolkit::opengl::primitives_heap<glm::vec2> ;
  heap_vertices_t heap_vertices;

 private:
  ModelViewProjection mvp;

 public:
  EasyTriangleHeapPainter() :
    heap_vertices( graphic_toolkit::opengl::attrib_pointer( 0, 2, raiigl::data_type::Float, true ) )
    //
  {

    //
    {
      heap_vertices_t::vertex_expander triangles( heap_vertices.complete_primitive( raiigl::primitive_type::Triangles ) );
      triangles.reserve( 3 );
      triangles.push( glm::vec2( -1.0f, -1.0f ) );
      triangles.push( glm::vec2( 1.0f, -1.0f ) );
      triangles.push( glm::vec2( 0.0f,  1.0f ) );
    }

    //
    heap_vertices.init_buffer();

  }

  float red_curve_count = 0.f;

  void paint( GLFWwindow * window ) {

    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_vertex_mvp.set( mvp.mvpRefresh() );

    //
    red_curve_count += 0.1f;
    const float red_curve( std::sin( red_curve_count ) * 0.333f + 0.666f );
    uniform_color.set( glm::vec3( red_curve, 0.f, 0.f ) );

    //
    heap_vertices.draw( gl330, program );

  }

};


int main()
{

  try
  {
    //
    Window win( 800, 600, "Draw Triangle GLFW Windows" );

    //
    win.painters.push_back( std::make_unique<EasyTriangleHeapPainter>() );

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

