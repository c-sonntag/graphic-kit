#pragma once

#include <gtk/window/abstract.hpp>

#include <string>
#include <memory>
#include <vector>

struct GLFWwindow;

namespace gtk {
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
      struct controller : public abstract_controller
      {
       protected:
        glfw& parent;
        friend glfw;
        inline controller( glfw& _parent ) : parent( _parent ) {}

       protected:
        window::key_modifier m_key_modifier = window::key_modifier::_none;
        bool glfw_key_pressed( uint k );
        void poll();

       public:
        void active_cursor( bool enable = true ) override;
        void set_cursor( const glm::uvec2& position ) override;
        bool key_pressed( const window::key& k ) override;
        window::key_modifier key_modifier() override;
      }
      controller{ *this };

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

     public:
      std::vector<test_function_pf_t> close_functions;

     public:
      glfw(
        gtk::render::painter_context& _context,
        const glfw_render_opengl_property& property,
        bool add_default_close = true
      );
      virtual ~glfw() override;

     protected:
      double current_time = 0.;
      double last_time = 0.;
      float delta_time = 0.;

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

#include <gtk/window/glfw.ipp>
