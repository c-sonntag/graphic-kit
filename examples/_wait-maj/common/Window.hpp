#pragma once

#include <AbstractPainter.hpp>

#include <gtools/frame_limiter.hpp>

#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


struct Window
{
 public:
  typedef bool ( *HandleTester_pf_t )( GLFWwindow * const w );

 public:
  GLFWwindow * const window;

 public:
  std::vector<std::unique_ptr<AbstractPainter>> painters;
  std::vector<HandleTester_pf_t> handleClose;

 private:
  gtools::frame_limiter fps_limiter{30};

 public:
  Window( uint32_t width, uint32_t height, const std::string & title );
  void run();

 private:
  static GLFWwindow * createWindow( uint32_t width, uint32_t height, const std::string & title );
};


// ----- ----- ----- -----


inline GLFWwindow * Window::createWindow( uint32_t width, uint32_t height, const std::string & title )
{
  //
  if ( !glfwInit() )
    throw std::runtime_error( "Failed to initialize GLFW from Window\n" );

  //
  glfwWindowHint( GLFW_SAMPLES, 4 ); // antialiasing 4x
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // Nous voulons OpenGL 3.3
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // Nous ne voulons pas de support de l'ancien OpenGL

  //
  GLFWwindow * const window
  {
    glfwCreateWindow(
      static_cast<int>( width ), static_cast<int>( height ),
      title.c_str(),
      NULL, NULL
    )
  };

  if ( window == nullptr )
  {
    glfwTerminate();
    throw std::runtime_error( "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
  }

  return window;
}


// ----- ----- ----- -----


inline Window::Window( uint32_t width, uint32_t height, const std::string & title ) :
  window( createWindow( width, height, title ) )
{

  //
  glfwMakeContextCurrent( window );

  // Initialise GLEW
  glewExperimental = true; // Nécessaire pour le profil core
  if ( glewInit() != GLEW_OK )
    throw std::runtime_error( "Failed to initialize GLEW\n" );

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );

  // Enable depth test
  glEnable( GL_DEPTH_TEST );
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc( GL_LESS );

  // Dark blue background
  glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );

}

inline void Window::run()
{
  //
  bool run = true;

  //
  while ( run )
  {
    fps_limiter.cadence();

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //
    for ( std::unique_ptr<AbstractPainter> & painter : painters )
      painter->paint( window );

    // Échange les tampons
    glfwSwapBuffers( window );
    glfwPollEvents();

    //
    for ( const HandleTester_pf_t & ht_pf : handleClose )
      if ( ht_pf( window ) )
      { run = false; break; }
  }
}
