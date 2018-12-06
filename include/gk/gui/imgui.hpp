#pragma once

#include <gk/matrices.hpp>
#include <gk/gui/abstract.hpp>

#include <imgui.h>

#include <string>

namespace ImGui {

  using uchar = unsigned char;
  using ushort = unsigned short;
  using uint = unsigned int;

  bool SliderUInt( const char* label, uint* v, uint v_min, uint v_max, const char* format = "%d" );
  bool ButtonStyle( const char* label, const ImVec4& c, const ImVec2& size = ImVec2(0, 0) );

  inline ImVec4& glm( glm::vec4& v ) { return reinterpret_cast<ImVec4&>( v ); }
  inline const ImVec4& glm( const glm::vec4& v ) { return reinterpret_cast<const ImVec4&>( v ); }

}

// ---- ---- ----  ----

namespace gk {
  namespace gui {
    namespace imgui {

      class abstract_style
      {
       public:
        virtual void apply( ImGuiStyle& style ) = 0;
      };

      // ---- ---- ---- ----

      namespace modal {
        void help_marker( const char* const disabled_txt, const char* desc );
      }

      // ---- ---- ---- ----

      void begin( const std::string& modal_name, const bool fixed );
      void end();

      // ---- ---- ---- ----

      void text( const float& v );
      void text( const glm::vec2& v );
      void text( const glm::vec3& v );
      void text( const glm::vec4& v );

      template<typename TVar, class TFunction>
      bool draging_button( TVar& var, const char* label, const TFunction& draging_func );

      template<typename TVar, class TFunction>
      bool draging_button_xy( TVar& var, const char* label, const TFunction& func );

      template<typename TVar, class TFunction>
      bool draging_button_x_or_y( TVar& var, const char* label, const TFunction& func, bool drag_on_y = false );

      // ---- ---- ---- ----

      namespace button_slider {
        bool xyz( glm::vec3& position );
      }

      // ---- ---- ---- ----

      namespace matrices {
        bool projection( const gui::abstract& g, gk::matrices::projection& projection, const char* const node_name = "Projection" );
        bool view( const gui::abstract& g, gk::matrices::view_lookat& view, const char* const node_name = "LookAt" );
        bool model( const gui::abstract& g, gk::matrices::model& model, const char* const node_name = "Model" );
      }

    }
  }
}

// ---- ---- ---- ----

#include <gk/gui/imgui.ipp>
