#pragma once

#include <gk/window/glfw.hpp>
#include <gk/window/glfw_controller.ipp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace gk {
  namespace window {

    inline GLFWwindow* glfw::create_window( const glfw_render_opengl_property& property )
    {
      //
      if( !glfwInit() )
        throw std::runtime_error( "[gk::windows::glfw] Failed to initialize GLFW\n" );

      //
      glfwWindowHint( GLFW_SAMPLES, property.antialiasing ); // antialiasing 4x

      //
      glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, property.major );
      glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, property.minor );

      if( property.core_profile )
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // We don't want old OpenGL support

#ifndef NDEBUG
      glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE ); /**< @todo garder pour le release ? */
#endif

      //
      GLFWwindow* const window
      {
        glfwCreateWindow(
          static_cast<int>( property.orginal_resolution.x ), static_cast<int>( property.orginal_resolution.y ),
          property.title.c_str(),
          nullptr, nullptr
        )
      };

      if( window == nullptr )
      {
        glfwTerminate();
        throw std::runtime_error( "[gk::windows::glfw] Failed to open GLFW window in "
          + std::to_string(property.major) + "." + std::to_string(property.minor) + " core profile.\n"
          + "Please check if your graphic card is compatible.\n" );
      }

      return window;
    }

    // ---- ---- ---- ----

    __forceinline glfw& glfw::get( GLFWwindow* w )
    {
      glfw* const t { static_cast<glfw * >( glfwGetWindowUserPointer( w ) ) };
      if( t ) return *t;
      else throw std::runtime_error( "[gk::window::glfw] No valid UserPointer" );
    }

    // ---- ---- ---- ----

    inline glfw::glfw(
      gk::render::painter_context& _context,
      const glfw_render_opengl_property& property,
      bool add_default_close
    ) :
      abstract( _context ),
      window( create_window( property ) )
    {
      // Config internal
      glfwSetWindowUserPointer( window, this );

      //
      glfwMakeContextCurrent( window );
      glfwSwapInterval( 1 ); // Enable vsync

      // Initialise GLEW
      glewExperimental = true; // Nécessaire pour le profil core
      if( glewInit() != GLEW_OK )
        throw std::runtime_error( "Failed to initialize GLEW\n" );

      //
      if( property.capture_key )
        glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );

      //
      if( property.cursor == glfw_context_property::cursor_mode::normal )
        glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
      else if( property.cursor == glfw_context_property::cursor_mode::hidden )
        glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
      else if( property.cursor == glfw_context_property::cursor_mode::disabled )
        glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

      //
      glfwSetFramebufferSizeCallback( window, callback::framebuffer_size );
      glfwSetWindowSizeCallback( window, callback::window_size );

      //
      if( property.capture_mouse )
      {
        glfwSetScrollCallback( window, glfw_controller::mouse_wheel_callback );
        glfwSetCursorPosCallback( window, glfw_controller::cursor_position_callback );
        // glfwSetMouseButtonCallback( window, glfw_controller::cursor_click_callback );
      }

      // //
      // imgui_support::init( window );

      // Enable depth test
      glEnable( GL_DEPTH_TEST );
      // Accept fragment if it closer to the camera than the former one
      glDepthFunc( GL_LESS );

      // Dark blue background
      glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );

      //
      if( add_default_close )
        push_close_function( [] ( GLFWwindow * window )
          { return glfwWindowShouldClose( window ) || ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ); } );
    }


    inline glfw::~glfw()
    {
      // imgui_support::clean( window );

      glfwDestroyWindow( window );
      glfwTerminate();
    }

    // ---- ---- ---- ----

    inline void glfw::callback::framebuffer_size( GLFWwindow* window, int width, int height )
    { glfw::callback::resize( window, width, height ); }

    inline void glfw::callback::window_size( GLFWwindow* window, int width, int height )
    { glfw::callback::resize( window, width, height ); }

    inline void glfw::callback::resize( GLFWwindow* window, int width, int height )
    {
      if( width >= 0 && height >= 0 )
      {
        glfw::get( window ).receiver.reshape( {width, height} );
        glfw::get( window ).m_screen_size = {width, height};
      }
      glfwSwapBuffers( window );
    }

    // ---- ---- ---- ----

    inline void glfw::poll()
    {
      m_controller.poll();
    }

    inline void glfw::clear()
    {
      int display_w, display_h;
      glfwGetFramebufferSize( window, &display_w, &display_h );
      glViewport( 0, 0, display_w, display_h );

      // Clear the screen
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    inline void glfw::swap()
    {
      glfwMakeContextCurrent( window );
      glfwSwapBuffers( window );
    }

    inline glfw_controller& glfw::controller()
    { return m_controller; }

    // ---- ----

    inline bool glfw::check_close()
    {
      for( const test_function_pf_t& ht_pf : close_functions )
        if( ht_pf( window ) )
          return true;
      return false;
    }

    // ---- ---- ---- ----

  }
}
