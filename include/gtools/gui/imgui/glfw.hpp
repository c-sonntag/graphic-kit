#pragma once

#include <gtools/gui/abstract.hpp>
#include <gtools/gui/imgui.hpp>

#include <gtools/gui/imgui/styles.hpp>

#include <memory>

struct GLFWwindow;

namespace gtools {
  namespace gui {
    namespace imgui {

      class glfw : public gtools::gui::abstract_handled<GLFWwindow>
      {
       public:
        using style_up_t = std::unique_ptr<abstract_style>;

       public:
        glfw( GLFWwindow* const window_handle, style_up_t _style_up = std::make_unique<styles::color_light>( ) );
        ~glfw() override;

       public:
        void new_frame() override;
        void render_frame() override;
        void draw_render() override;

       protected:
        style_up_t style_up; /** @todo it's debug style */

       public:
        void set_default( style_up_t _style_up );

      };

    }
  }
}

// ---- ---- ---- ----

#include <gtools/gui/imgui/glfw.ipp>
