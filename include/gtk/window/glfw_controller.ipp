#pragma once

#include <gtk/window/glfw_controller.hpp>

#include <gtk/window/glfw.hpp>

#include <GLFW/glfw3.h>

#include <gtk/debug.hpp>

namespace gtk {
  namespace window {

    inline bool glfw_controller::glfw_key_pressed( uint k )
    { return glfwGetKey( parent.window, int(k) ) == GLFW_PRESS; }

    inline void glfw_controller::poll()
    {
      glfwPollEvents();

      //
      m_key_modifier =
        window::key_modifier::_none
        | ( glfw_key_pressed( GLFW_KEY_LEFT_SHIFT )    * window::key_modifier::ShiftLeft )
        | ( glfw_key_pressed( GLFW_KEY_RIGHT_SHIFT )   * window::key_modifier::ShiftRight )
        | ( glfw_key_pressed( GLFW_KEY_LEFT_CONTROL )  * window::key_modifier::ControlLeft )
        | ( glfw_key_pressed( GLFW_KEY_RIGHT_CONTROL ) * window::key_modifier::ControlRight )
        | ( glfw_key_pressed( GLFW_KEY_LEFT_ALT )      * window::key_modifier::AltLeft )
        | ( glfw_key_pressed( GLFW_KEY_RIGHT_ALT )     * window::key_modifier::AltRight );

    }

    // ---- ----

    inline void glfw_controller::set_cursor_position( const glm::dvec2& position )
    { glfwSetCursorPos( parent.window, position.x, position.y ); }

    inline const glm::uvec2& glfw_controller::screen_size() const
    { return parent.screen_size(); }

    inline void glfw_controller::active_cursor( bool enable )
    {
      if( enable ) glfwSetInputMode( parent.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
      else glfwSetInputMode( parent.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    // ---- ---- ---- ----

    bool glfw_controller::is_clicked( const window::mouse_button& b )
    {
      if( any( b & window::mouse_button::left ) )
        if( glfwGetMouseButton( parent.window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
          return true;

      if( any( b & window::mouse_button::middle ) )
        if( glfwGetMouseButton( parent.window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS )
          return true;

      if( any( b & window::mouse_button::right ) )
        if( glfwGetMouseButton( parent.window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS )
          return true;

      return false;
    }

    // ---- ---- ---- ----

    inline void glfw_controller::mouse_wheel_callback( GLFWwindow* window, double xoffset, double yoffset )
    {
      glfw::get( window ).controller().m_mouse_wheel += glm::dvec2( xoffset, yoffset );

      debug_cout << xoffset << " " << yoffset << std::endl;
    }

    inline void glfw_controller::cursor_position_callback( GLFWwindow* window, double xoffset, double yoffset )
    { glfw::get( window ).controller().m_cursor_position = { xoffset, yoffset };  }

    // inline void glfw_controller::cursor_click_callback( GLFWwindow* window, int button, int action, int mods )
    // {
    //   glfw::get( window ).controller().m_cursor_click = { v1, v2, v3 };
    //   debug_cout << "click(" << glm::uvec3( v1, v2, v3 ) << std::endl;
    // }


  }
}
