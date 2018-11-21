#include <gtk/render/painter_context.hpp>
#include <gtk/window/glfw.hpp>
#include <gtk/window/command/mouse_lookat_center.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/vertex_array.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/gl330.hpp>

#include <gtk/opengl/quick_program.hpp>

#include <erc/package_id.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

#include <memory>
#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


static raiigl::gl330 gl;

struct triangle_red_min_painter : public gtk::render::painter::abstract
{
 private:
  const raiigl::program program
  {
    gtk::opengl::quick_program::open_from_sources(
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
  raiigl::buffer triangle_buffer{ raiigl::buffer_type::Array, raiigl::buffer_usage::StaticDraw };

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };
  const raiigl::uniform_variable uniform_color{ program, "uniform_color" };

 public:
  triangle_red_min_painter( gtk::matrices::projection& _projection ) :
    abstract( _projection )
  {
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

  void paint() override
  {
    program.use();

    camera.compute();
    uniform_vertex_mvp.set( camera );

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
  gtk::window::glfw_render_opengl_property windows_property{};
  windows_property.orginal_resolution = { 800, 600 };
  windows_property.title = "Draw Triangle red min";
  windows_property.antialiasing = 4;
  windows_property.major = 3;
  windows_property.minor = 3;
  windows_property.core_profile = true;


  try {

    //
    gtk::render::painter_context context;
    gtk::window::glfw glfw_window( context, windows_property );

    //
    auto& painter( context.push_painter<triangle_red_min_painter>( context.projection ) );
    painter.push_command<gtk::window::command::mouse_lookat_center>( glfw_window.controller(), gtk::window::key_modifier::Control );

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

