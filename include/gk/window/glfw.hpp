#pragma once

#include <gk/window/abstract.hpp>
#include <gk/window/glfw_controller.hpp>

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace gk {
  namespace window {

    struct glfw_context_property
    {

      /**
       * @group Window context
       * @{
       */

      enum class cursor_mode { normal = 0, hidden, disabled };

      glm::uvec2 orginal_resolution{ 800, 600 };
      std::string title{ "unknown window name" };
      uint antialiasing{ 4 };

      cursor_mode cursor{ cursor_mode::normal };

      /**
       * }@
       * @group Inputs :
       * @{
       */

      bool capture_key{ true };
      bool capture_mouse{ true };

      /**
       * }@
       */

    };

    struct glfw_render_opengl_property : public glfw_context_property
    {
      uint major{ 3 }, minor{ 3 };
      bool core_profile{ true };
    };

    // struct glfw_render_vulkan_property

    // ---- ---- ---- ----

    struct glfw : public abstract
    {
     protected:
      glfw_controller m_controller{ *this };

     public:
      typedef bool (* test_function_pf_t)( GLFWwindow* const w );

     private:
      static GLFWwindow* create_window( const glfw_render_opengl_property& property );
      // static GLFWwindow* create_window( const glfw_render_vulkan_property & property );

     public:
      //glfw_render_opengl_property* const mode;
      // glfw_render_vulkan_property * const mode;

     public:
      GLFWwindow* const window;
      static glfw& get( GLFWwindow* w );

     public:
      std::vector<test_function_pf_t> close_functions;

     public:
      glfw(
        gk::render::painter_context& _context,
        const glfw_render_opengl_property& property,
        bool add_default_close = true
      );
      virtual ~glfw() override;

     protected:
      glm::uvec2 m_screen_size;

     public:
      const glm::uvec2& screen_size() const { return m_screen_size; }

     public:
      virtual void poll() override;
      virtual void clear() override;
      virtual void swap() override;

     public:
      virtual glfw_controller& controller() override;

     public:
      virtual bool check_close() override;

      // public:
      //  virtual void* handle() override { return window; }
      //  virtual const void* handle() const override { return window; }

     private:
      struct callback
      {
       public:
        static void error( int error, const char* description );

       public:
        static void framebuffer_size( GLFWwindow* window, int width, int height );
        static void window_size( GLFWwindow* window, int width, int height );
        static void resize( GLFWwindow* window, int width, int height );
      };

     public:
      inline void push_close_function( const test_function_pf_t&& ht )
      { close_functions.emplace_back( ht ); }

    };

  }
}

// ---- ---- ---- ----

#include <gk/window/glfw.ipp>
