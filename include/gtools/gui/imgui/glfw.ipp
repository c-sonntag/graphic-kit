#pragma once

#include <gtools/gui/imgui/glfw.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gtools {
  namespace gui {
    namespace imgui {

      static const char* glsl_version = "#version 150";

      inline glfw::glfw( GLFWwindow* const _window_handle, style_up_t _style_up ) :
        abstract_handled( _window_handle ),
        style_up( std::move( _style_up ) )
      {
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        ImGui_ImplGlfw_InitForOpenGL( window_handle, true );
        ImGui_ImplOpenGL3_Init( glsl_version );

        // Setup style
        if( style_up )
          style_up->apply( ImGui::GetStyle() );
      }

      inline glfw::~glfw()
      {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
      }

      // ---- ---- ---- ----

      void glfw::new_frame()
      {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
      }

      void glfw::render_frame()
      { ImGui::Render(); }

      void glfw::draw_render()
      { ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() ); }

      // ---- ---- ---- ----

      inline void glfw::set_default( style_up_t _style_up )
      {
        style_up = std::move( _style_up );
        if( style_up )
          style_up->apply( ImGui::GetStyle() );
      }

    }
  }
}
