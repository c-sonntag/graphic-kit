#pragma once

#include <gk/gui/imgui.hpp>

#include <memory>

struct GLFWwindow;

namespace gk {
  namespace gui {
    namespace imgui {
      namespace styles {


        struct color_light : public imgui::abstract_style
        {
          virtual void apply( ImGuiStyle& style ) override
          {
            ImGui::StyleColorsLight();
            style.FrameBorderSize = 1.f;
            //style.
          }
        };

      }
    }
  }
}
