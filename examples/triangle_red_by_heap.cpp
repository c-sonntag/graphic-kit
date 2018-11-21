#include <gtk/render/painter_context.hpp>
#include <gtk/window/glfw.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/gl330.hpp>

#include <gtk/opengl/quick_program.hpp>
#include <gtk/opengl/primitives_heap.hpp>

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

struct easy_triangle_heap_painter : public gtk::render::painter::abstract
{
 private:
  raiigl::gl330 gl330;
  raiigl::program program
  {
    gtk::opengl::quick_program::open_from_local_erc(
      shader_erc_id.from( "shader.vert" ),
      shader_erc_id.from( "shader.frag" )
    )
  };

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };
  const raiigl::uniform_variable uniform_color{ program, "uniform_color" };

 private:
  using heap_vertices_t = gtk::opengl::primitives_heap<glm::vec2>;
  heap_vertices_t heap_vertices;

 public:
  easy_triangle_heap_painter( gtk::matrices::projection& _projection ) :
    abstract( _projection ),
    heap_vertices( gtk::opengl::attrib_pointer( 0, 2, raiigl::data_type::Float, true ) )
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

  void paint() override
  {

    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_vertex_mvp.set( camera );

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
  gtk::window::glfw_render_opengl_property windows_property{};
  windows_property.orginal_resolution = { 800, 600 };
  windows_property.title = "Draw Triangle GLFW Windows";
  windows_property.antialiasing = 4;
  windows_property.major = 3;
  windows_property.minor = 3;
  windows_property.core_profile = true;

  try {

    //
    gtk::render::painter_context context;
    gtk::window::glfw glfw_window( context, windows_property );

    //
    context.push_painter<easy_triangle_heap_painter>( context.projection );

    //
    glfw_window.run();

  }
  catch( const std::exception& e )
  {
    std::cerr << "Error : " <<  e.what() << std::endl;
    return -1;
  }

  return 0;
}

