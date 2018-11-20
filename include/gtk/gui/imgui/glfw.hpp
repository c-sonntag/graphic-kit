#pragma once

#include <gtk/gui/abstract.hpp>
#include <gtk/gui/imgui.hpp>

#include <gtk/gui/imgui/styles.hpp>

#include <memory>

struct GLFWwindow;

namespace gtk {
  namespace gui {
    namespace imgui {

      class glfw : public gtk::gui::abstract_handled<GLFWwindow>
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

#include <gtk/gui/imgui/glfw.ipp>
