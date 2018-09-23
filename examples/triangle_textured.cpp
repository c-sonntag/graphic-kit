#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/texture.hpp>

#include <graphic_toolkit/image.h>
#include <graphic_toolkit/opengl/texture.h>
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

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const erc::package_id resource_erc_id( "res" );

struct image_with_texture
{
  const erc::file_id erc_id;
  const graphic_toolkit::image img;
  const raiigl::texture tex;

  inline image_with_texture( const erc::file_id _erc_id ) :
    erc_id( std::move( _erc_id ) ),
    img( graphic_toolkit::image::load_from_local_erc( erc_id ) ),
    tex( graphic_toolkit::opengl::texture_from_image( img ) )
  {}

  inline void print_info() {
    std::cout << "Image '" << erc_id.file_path << "' info : " << std::endl
              << "    dimension : " << img.width << "x" << img.height << " by " << img.channels << "bits" << std::endl
              << "    size      : " << img.size << std::endl
              << "    is_flip ? : " << std::boolalpha << img.is_vertical_flip << std::endl
              << std::endl ;
  }
};





struct EasyTriangleHeapPainter : public AbstractPainter
{
 private:
  const raiigl::gl330 gl330;
  raiigl::program program
  {
    graphic_toolkit::opengl::quick_program::open_from_local_erc(
      resource_erc_id.from( "shader.vert" ),
      resource_erc_id.from( "shader.frag" )
    )
  };

 private:
  const raiigl::uniform_variable uniform_mvp{ program, "uniform_mvp" };
  const raiigl::uniform_variable uniform_model_decal{ program, "uniform_model_decal" };
  const raiigl::uniform_variable sampler_texture{ program, "sampler_texture" };

 private:
  using heap_vertices_t = graphic_toolkit::opengl::primitives_heap<glm::vec2, glm::vec2> ;
  heap_vertices_t heap_vertices;

 private:
  image_with_texture iwt_transparent{resource_erc_id.from( "texture_transparent.png" )};
  image_with_texture iwt_opaque{resource_erc_id.from( "texture_opaque.png" )};

 private:
  ModelViewProjection mvp;

 public:
  EasyTriangleHeapPainter() :
    heap_vertices(
      graphic_toolkit::opengl::attrib_pointer( 0, 2, raiigl::data_type::Float, true ),
      graphic_toolkit::opengl::attrib_pointer( 1, 2, raiigl::data_type::Float, true )
    ) {

    uniform_model_decal.set( glm::vec2( 0.f, 0.f ) );

    //
    gl330.enable( raiigl::gl_capabilities::Blend );
    gl330.blend_func( raiigl::blend_func_type::SrcAlpha, raiigl::blend_func_type::OneMinusSrcAlpha );

    //
    iwt_transparent.print_info();
    iwt_opaque.print_info();

    //
    const raiigl::textures_num iwt_transparent_sampler_id( raiigl::textures_num::Texture01 );
    const raiigl::textures_num iwt_opaque_sampler_id( raiigl::textures_num::Texture02 );
    iwt_transparent.tex.bind_on_texture( iwt_transparent_sampler_id );
    iwt_opaque.tex.bind_on_texture( iwt_opaque_sampler_id );

    //gl330.activate_texture( iwt_transparent_sampler_id );
    //iwt_transparent.tex.bind();
    //gl330.activate_texture( iwt_opaque_sampler_id );
    //iwt_opaque.tex.bind();

    //
    //gl330.activate_texture( raiigl::textures_num::Texture0 );

    const auto expander_triangles_push( []( heap_vertices_t::vertex_expander & triangles ) {
      triangles.reserve( 3 );
      triangles.push( glm::vec2( -1.0f, -1.0f ), glm::vec2( 0.f, 1.f ) );
      triangles.push( glm::vec2( 1.0f, -1.0f ), glm::vec2( 1.f, 1.f ) );
      triangles.push( glm::vec2( 0.0f,  1.0f ), glm::vec2( 0.5f, 0.f ) );
    } );

    //
    {
      heap_vertices_t::vertex_expander triangles( heap_vertices.complete_primitive( raiigl::primitive_type::Triangles ) );
      expander_triangles_push( triangles );
      triangles.set_uniform( "uniform_model_decal", glm::vec2( -1.f, 0.f ) );
      triangles.set_uniform( "sampler_texture",  iwt_opaque_sampler_id );
    }

    //
    {
      heap_vertices_t::vertex_expander triangles( heap_vertices.complete_primitive( raiigl::primitive_type::Triangles ) );
      expander_triangles_push( triangles );
      triangles.set_uniform( "uniform_model_decal", glm::vec2( 1.f, 0.f ) );
      triangles.set_uniform( "sampler_texture", iwt_transparent_sampler_id );
    }

    //
    heap_vertices.init_buffer();

  }

  float red_curve_count = 0.f;

  void paint( GLFWwindow * ) {

    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_mvp.set( mvp.mvpRefresh() );

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

