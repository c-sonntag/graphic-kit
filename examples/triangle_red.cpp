#include <AbstractPainter.hpp>
#include <ModelViewProjection.hpp>
#include <Window.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/program.hpp>
#include <raiigl/uniform_variable.hpp>

#include <graphic_toolkit/opengl/quick_program.h>

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

struct EasyTrianglePainter : public AbstractPainter
{
 private:
  const raiigl::program program
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
  GLuint vertex_buffer;
  GLuint vertex_array_id;

 private:
  ModelViewProjection mvp;

 private:
  std::chrono::system_clock::time_point start;

 public:
  EasyTrianglePainter() {

    start = std::chrono::system_clock::now();

    //
    glGenVertexArrays( 1, &vertex_array_id );
    glBindVertexArray( vertex_array_id );

    // Un tableau de trois vecteurs qui représentent les trois sommets d'un triangle
    static const GLfloat g_vertex_buffer_data[]
    {
      -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f
      };

    // Ceci identifiera notre tampon de sommets


    // Génère un tampon et place l'identifiant dans 'vertex_buffer'
    glGenBuffers( 1, &vertex_buffer );

    // Les commandes suivantes vont parler de notre tampon 'vertex_buffer'
    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );

    // Fournit les sommets à OpenGL.
    glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );

  }

  float red_curve_count = 0.f;

  void paint( GLFWwindow * window ) {


    // Utilise notre shader
    program.use();

    // Send our transformation
    uniform_vertex_mvp.set( mvp.mvpRefresh() );

    //
    red_curve_count += 0.01f;
    const float red_curve( std::sin( red_curve_count ) * 0.333f + 0.666f );
    uniform_color.set( glm::vec3( red_curve, 0.f, 0.f ) );

    // premier tampon d'attributs : les sommets
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
    glVertexAttribPointer(
      0,                  // attribut 0. Aucune raison particulière pour 0, mais cela doit correspondre au « layout » dans le shader
      3,                  // taille
      GL_FLOAT,           // type
      GL_FALSE,           // normalisé ?
      0,                  // nombre d'octets séparant deux sommets dans le tampon
      ( void * )0         // décalage du tableau de tampon
    );


    // Dessine le triangle !
    glDrawArrays( GL_TRIANGLES, 0, 3 ); // Démarre à partir du sommet 0; 3 sommets au total -> 1 triangle

    // glDrawArrays( GL_LINE_LOOP, 0, 3 ); // Démarre à partir du sommet 0; 3 sommets au total -> 1 triangle

    glDisableVertexAttribArray( 0 );

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

