#pragma once

#include <gtools/window/abstract.hpp>

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace gtools {
  namespace window {

    struct glfw_render_property
    {
      glm::uvec2 orginal_resolution{ 800, 600 };
      std::string title{ "unknown window name" };
      uint antialiasing{ 4 };
    };

    struct glfw_render_opengl_property : public glfw_render_property
    {
      uint major{ 3 }, minor{ 3 };
      bool core_profile{ true };
    };

    // struct glfw_render_vulkan_property

    // ---- ---- ---- ----

    struct glfw : public abstract
    {
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

     public:
      std::vector<test_function_pf_t> close_functions;

     public:
      glfw(
        gtools::render::painter_context& _context,
        const glfw_render_opengl_property& property,
        bool add_default_close = true
      );
      virtual ~glfw() override;

     public:
      virtual void poll() override;
      virtual void clear() override;
      virtual void swap() override;

     public:
      virtual bool check_close() override;

      // public:
      //  virtual void* handle() override { return window; }
      //  virtual const void* handle() const override { return window; }

     private:
      struct callback
      {
       public:
        static __forceinline glfw& get( GLFWwindow* w );

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

#include <gtools/window/glfw.ipp>
