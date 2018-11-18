#pragma once

#include <gtools/gui/abstract.hpp>

#include <memory>

namespace window {
  namespace gui {

    class imgui_style
    {
     public:
      virtual void apply( ImGuiStyle& style );
    };

    class glfw_imgui : public abstract<GLFWwindow>
    {
     public:
      glfw_imgui( GLFWwindow* const window_handle );
      ~glfw_imgui();

     protected:
      virtual void default_stylise();

     protected:
      std::unique_ptr<imgui_style> style_up; /** @todo it's ddebug style */

    public:
      inline void set_default( const TMatrice & d ) {
        default_m_up = std::make_unique<TMatrice>( d );
        default_m_up->compute();
      }

    };

  }
}
