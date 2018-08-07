#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/texture.hpp>

#include <graphic_toolkit/image.h>
#include <graphic_toolkit/base_format.h>
#include <graphic_toolkit/opengl/quick_program.h>
#include <graphic_toolkit/opengl/primitives_heap.h>

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
#include <glm/vec2.hpp>

static const erc::package_id resource_erc_id( "res" );

struct EasyTriangleHeapPainter : public AbstractPainter
{
 private:
  const raiigl::gl330 gl330;
  const std::unique_ptr<raiigl::program> program_up
  {
    graphic_toolkit::opengl::quick_program::open_from_local_erc(
      resource_erc_id.from( "shader.vert" ),
      resource_erc_id.from( "shader.frag" )
    )
  };

 private:
  const raiigl::uniform_variable uniform_vertex_mvp{ *program_up, "MVP" };

 private:
  using heap_vertices_t = graphic_toolkit::opengl::primitives_heap<glm::vec2, glm::vec2> ;
  heap_vertices_t heap_vertices;

 private:
  std::unique_ptr<graphic_toolkit::image> tex_img_up
  {
    graphic_toolkit::image::load_from_local_erc( resource_erc_id.from( "texture.png" ) )
  };
  raiigl::texture tex{raiigl::texture_type::Texture2D};

 private:
  ModelViewProjection mvp;

 private:
  std::chrono::system_clock::time_point start;

 public:
  EasyTriangleHeapPainter() :
    heap_vertices(
      graphic_toolkit::opengl::attrib_pointer( 0, 2, raiigl::data_type::Float, true ),
      graphic_toolkit::opengl::attrib_pointer( 1, 2, raiigl::data_type::Float, true )
    ) {

    //
    const bool good_image_format(
      graphic_toolkit::base_format::is_pow_of( 2u, tex_img_up->width ) &&
      graphic_toolkit::base_format::is_pow_of( 2u, tex_img_up->height )
    );
    if ( !good_image_format )
      std::cerr << "Warning, incompatible texture size format : " << tex_img_up->width << "x" << tex_img_up->height << std::endl;


    std::cout << "Image info : " << std::endl
              << "    dimension : " << tex_img_up->width << "x" << tex_img_up->height << " by " << tex_img_up->channels << "bits" << std::endl
              << "    size      : " << tex_img_up->size << std::endl
              << "    is_flip ? : " << std::boolalpha << tex_img_up->is_vertical_flip << std::endl
              << std::endl ;

    //
    start = std::chrono::system_clock::now();

    //
    {
      heap_vertices_t::vertex_expander triangles( heap_vertices.complete_primitive( raiigl::primitive_type::Triangles ) );
      triangles.reserve( 3 );
      triangles.push( glm::vec2( -1.0f, -1.0f ), glm::vec2( 0.f, 1.f ) );
      triangles.push( glm::vec2( 1.0f, -1.0f ), glm::vec2( 1.f, 1.f ) );
      triangles.push( glm::vec2( 0.0f,  1.0f ), glm::vec2( 0.5f, 0.f ) );
    }

    //
    heap_vertices.init_buffer();

    //
    tex.bind();
    tex.send_image2d(
      0,
      raiigl::internal_format_type::RGBA,
      tex_img_up->width, tex_img_up->height,
      raiigl::pixel_format::RGBA,
      raiigl::pixel_type::UnsignedByte,
      tex_img_up->data()
    );
    tex.set_param_minifying_and_magnification_filter(
      raiigl::minifying_filter_type::Nearest,
      raiigl::magnification_filter_type::Nearest
    );


  }

  float red_curve_count = 0.f;

  void paint( GLFWwindow * ) {

    // Utilise notre shader
    program_up->use();

    // Send our transformation
    uniform_vertex_mvp.set( mvp.mvpRefresh() );

    //
    heap_vertices.draw( gl330, *program_up );

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

