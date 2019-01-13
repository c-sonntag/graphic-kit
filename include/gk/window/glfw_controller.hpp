#pragma once

#include <gk/window/abstract.hpp>

#include <gk/enum_bits.hpp>

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace gk {
  namespace window {

    struct glfw;

    struct glfw_controller : public abstract_controller
    {
     protected:
      glfw& parent;
      friend glfw;
      inline glfw_controller( glfw& _parent ) : parent( _parent ) {}

     protected:
      window::modifier m_modifier = window::modifier::_none;
      glm::dvec2 m_mouse_wheel;
      glm::dvec2 m_cursor_position;

      //glm::bvec3 m_cursor_click;
     protected:
      bool glfw_key_pressed( uint k );
      void poll();

     public:
      void active_cursor( bool enable = true ) override;

      inline void set_cursor_position( const glm::dvec2& position ) override;

      inline const glm::uvec2& screen_size() const override;

      inline const glm::dvec2& cursor_position() const override
      { return m_cursor_position; }

      inline const glm::dvec2& mouse_wheel() const override
      { return m_mouse_wheel; }

      bool is_clicked( const window::mouse_button& b ) override;

      inline bool is_pressed( const window::key& k ) override
      { return glfw_key_pressed( uint( k ) ); }

      inline window::modifier modifier() override
      { return m_modifier; }

     protected:
      static void mouse_wheel_callback( GLFWwindow* window, double xoffset, double yoffset );
      static void cursor_position_callback( GLFWwindow* window, double xoffset, double yoffset );
      // static void cursor_click_callback( GLFWwindow* window, int button, int action, int mods );

    };

  }
}

// // ---- ---- ---- ----
//
// #include <gk/window/glfw_controller.hpp>
