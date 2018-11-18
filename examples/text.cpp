#include <gtools/render/painter_context.hpp>
#include <gtools/window/glfw.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <gtools/basic_colors.hpp>
#include <gtools/opengl/quick_program.hpp>
#include <gtools/opengl/quick_text.hpp>
#include <gtools/opengl/quick_text_expander.hpp>

#include <erc/package_id.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

#include <memory>
#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

static const erc::package_id shader_erc_id( "shaders" );

struct text_painter : public gtools::render::painter::abstract
{
 private:
  const raiigl::program program
  {
    gtools::opengl::quick_program::open_from_local_erc(
      shader_erc_id.from( "shader.vert" ),
      shader_erc_id.from( "shader.frag" )
    )
  };

  gtools::opengl::quick_text calibri{ gtools::opengl::quick_text_fonts::CalibriLight_1024 };

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ program, "MVP" };

 private:
  raiigl::gl330 gl;

 public:
  text_painter( gtools::matrices::projection& _projection ) :
    abstract( _projection )
  {

    {
      gtools::opengl::text_expander yop( calibri.complete_text( "Yop ! Yap !" ) );
      yop.color = gtools::basic_colors::intense_green;
      yop.align_h = gtools::opengl::text_expander::center;
      yop.normal_size = 1.f;
    }

  }

  void paint() override
  {

    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_vertex_mvp.set( camera );

    //
    calibri.draw( gl, camera );
  }

};


int main()
{
  gtools::window::glfw_render_opengl_property windows_property{};
  windows_property.orginal_resolution = { 800, 600 };
  windows_property.title = "Draw Text on GLFW Windows";
  windows_property.antialiasing = 4;
  windows_property.major = 3;
  windows_property.minor = 3;
  windows_property.core_profile = true;


  try {

    //
    gtools::render::painter_context context;
    gtools::window::glfw glfw_window( context, windows_property );

    //
    context.push_painter( std::make_unique<text_painter>( context.projection ) );

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
