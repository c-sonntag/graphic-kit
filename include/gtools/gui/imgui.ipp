#pragma once

#include <gtools/gui/imgui.hpp>

namespace ImGui {

  inline bool SliderUInt( const char* label, uint* v, uint v_min, uint v_max, const char* format )
  { return SliderScalar( label, ImGuiDataType_U32, v, &v_min, &v_max, format ); }

  inline bool ButtonStyle( const char* label, const ImVec4& c, const ImVec2& size )
  {
    constexpr float HC { 0.1f }, AC { 0.2f };
    ImGui::PushStyleColor( ImGuiCol_Button, c );
    ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( c.x + HC, c.y + HC, c.z + HC, c.w + HC ) );
    ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( c.x + AC, c.y + AC, c.z + AC, c.w + AC ) );
    const bool r { ImGui::Button( label, size ) };
    ImGui::PopStyleColor( 3 );
    return r;
  }

}

// ---- ---- ----  ----

namespace gtools {
  namespace gui {
    namespace imgui {

      namespace modal {

        inline void help_marker( const char* const disabled_txt, const char* desc )
        {
          ImGui::TextDisabled( "%s", disabled_txt );
          if( ImGui::IsItemHovered() )
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
            ImGui::TextUnformatted( desc );
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
          }
        }

      }

      // ---- ---- ---- ----

      inline void begin( const std::string& modal_name, const bool fixed )
      {
        //ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0 );

        ImGui::SetNextWindowPos( ImVec2( 10, 10 ), ( fixed ? ImGuiCond_Always : ImGuiCond_Appearing ) );
        ImGui::SetNextWindowSize( ImVec2( 400, 100 ), ImGuiCond_Appearing );

        const ImGuiWindowFlags window_flags(
          ImGuiWindowFlags_AlwaysAutoResize |
          ( fixed ? ImGuiWindowFlags_NoMove : 0 )
          // ImGuiWindowFlags_NoResize
        );

        ImGui::Begin( modal_name.c_str(), nullptr, window_flags );

        ImGui::Text( "FPS : %.1f", double(ImGui::GetIO().Framerate) );
      }

      inline void end()
      {
        ImGui::End();
      }

      // ---- ---- ---- ----

      static constexpr float mouse_ratio { 0.01f };
      static const ImVec4 reset_button_color { ImColor::HSV( 0.f, 1.f, 0.66f ) };

      inline void text( const float& v )      { ImGui::Text( "(%3.3f)", double(v) ); }
      inline void text( const glm::vec2& v )  { ImGui::Text( "(%3.3f,%3.3f)", double(v.x), double(v.y) ); }
      inline void text( const glm::vec3& v )  { ImGui::Text( "(%3.3f,%3.3f,%3.3f)", double(v.x), double(v.y), double(v.z) ); }
      inline void text( const glm::vec4& v )  { ImGui::Text( "(%3.3f,%3.3f,%3.3f,%3.3f)", double(v.x), double(v.y), double(v.z), double(v.w) ); }

      template<typename TVar, class TFunction>
      inline bool draging_button( TVar& var, const char* label, const TFunction& draging_func )
      {
        const ImGuiIO& io( ImGui::GetIO() );
        ImGui::Button( label );
        static bool is_actived { false };
        if( ImGui::IsItemActive() )
        {

          //
          static TVar original_var;
          if( !is_actived ) { original_var = var; is_actived = true; }
          else if( !io.KeyCtrl ) var = original_var;

          //
          draging_func();

          //
          ImGui::BeginTooltip();
          ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
          text( var );
          ImGui::PopTextWrapPos();
          ImGui::EndTooltip();

          return true;
        }
        else if( is_actived && io.MouseReleased[0] ) is_actived = false;
        return false;
      }

      template<typename TVar, class TFunction>
      inline bool draging_button_xy( TVar& var, const char* label, const TFunction& func )
      {
        const ImGuiIO& io( ImGui::GetIO() );
        return draging_button( var, label, [&] ()
          {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->PushClipRectFullScreen();
            draw_list->AddLine( io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32( ImGuiCol_Button ), 4.0f );
            draw_list->PopClipRect();
            ImVec2 value_raw = ImGui::GetMouseDragDelta( 0, 0.0f );
            func( var, value_raw.x, value_raw.y );
          } );
      }

      template<typename TVar, class TFunction>
      inline bool draging_button_x_or_y( TVar& var, const char* label, const TFunction& func, bool drag_on_y )
      {
        const ImGuiIO& io( ImGui::GetIO() );
        return draging_button( var, label, [&] ()
          {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->PushClipRectFullScreen();
            const ImVec2 click_pos { io.MouseClickedPos[0] };
            const ImVec2 drag_line { ( !drag_on_y ? io.MousePos.x : click_pos.x ), ( drag_on_y ? io.MousePos.y : click_pos.y ) };
            draw_list->AddLine( click_pos, drag_line, ImGui::GetColorU32( ImGuiCol_Button ), 4.0f );
            draw_list->PopClipRect();
            ImVec2 value_raw = ImGui::GetMouseDragDelta( 0, 0.0f );
            func( var, drag_on_y ? value_raw.y : value_raw.x );
          } );
      }

      // ---- ---- ---- ----

      inline in_circle_property::in_circle_property( const float x, const float y, const float x_center, const float y_center ) :
        base_x( x - x_center ), base_y( y - y_center ),
        rayon( std::sqrt( base_x * base_x + base_y * base_y ) + 0.0001f ),
        part( std::atan2( base_y, base_x ) ) {}

      // ---- ----

      inline void dist_xyz( glm::vec3& position, const float x )
      {
        in_circle_property in_circle_xz( position.x, position.z );
        in_circle_property in_circle_xy( position.x, position.y );
        const float decal { x* mouse_ratio };
        position.x = std::cos( in_circle_xz.part ) * ( in_circle_xz.rayon + decal );
        position.z = std::sin( in_circle_xz.part ) * ( in_circle_xz.rayon + decal );
        position.y = std::sin( in_circle_xy.part ) * ( in_circle_xy.rayon + decal );
      }

      inline void rot_xz( glm::vec3& position, const float x )
      {
        in_circle_property in_circle( position.x, position.z );
        const float decal { x* mouse_ratio };
        position.x = std::cos( in_circle.part + decal ) * in_circle.rayon;
        position.z = std::sin( in_circle.part + decal ) * in_circle.rayon;
      }

      inline void rot_zy( glm::vec3& position, const float x )
      {
        in_circle_property in_circle( position.z, position.y );
        const float decal { x* mouse_ratio };
        position.z = std::cos( in_circle.part + decal ) * in_circle.rayon;
        position.y = std::sin( in_circle.part + decal ) * in_circle.rayon;
      }

      // ---- ---- ---- ----

      namespace button_slider {

        inline bool xyz( glm::vec3& position )
        {
          bool change( false );
          change |= draging_button_xy( position, "XY", [&] ( auto& position, auto x, auto y ) { position.x += x * mouse_ratio; position.y += y * mouse_ratio; } );
          ImGui::SameLine();
          change |= draging_button_x_or_y( position, "Z", [&] ( auto& position, auto z ) { position.z += z * mouse_ratio; } );
          return change;
        }

      }

      // ---- ---- ---- ----

      namespace matrices {

        inline bool projection( gtools::matrices::projection& projection, const char* const node_name )
        {
          bool change( false );
          if( ImGui::TreeNode( node_name ) )
          {
            {
              change |= ImGui::Checkbox( "Ortho?", &projection.ortho_mode );
              change |= draging_button_x_or_y( projection.zoom, "Zoom", [&] ( auto& zoom, auto x ) { zoom += x * mouse_ratio; } );
              ImGui::SameLine();
              change |= draging_button_x_or_y( projection.fov, "Fov", [&] ( auto& fov, auto x ) { fov += x * mouse_ratio; } );

              if( change )
                projection.compute();
              if( ImGui::ButtonStyle( "Reset", reset_button_color ) )
                projection.reset();
            }
            ImGui::TreePop();
          }
          return change;
        }

        inline bool view( gtools::matrices::view_lookat& view, const char* const node_name )
        {
          bool change( false );
          if( ImGui::TreeNode( node_name ) )
          {
            {
              {
                change |= draging_button_xy( view.position, "XY", [&] ( auto& position, auto x, auto y ) { position.x += x * mouse_ratio; position.y += y * mouse_ratio; } );
                ImGui::SameLine();
                change |= draging_button_x_or_y( view.position, "Z", [&] ( auto& position, auto z ) { position.z += z * mouse_ratio; } );
              }
              {
                change |= draging_button_x_or_y( view.position, "D(XYZ)", dist_xyz );
              }
              {
                change |= draging_button_x_or_y( view.position, "R(XZ)", rot_xz );
                ImGui::SameLine();
                change |= draging_button_x_or_y( view.position, "R(ZY)", rot_zy, true );
              }

              if( change )
                view.compute();
              if( ImGui::ButtonStyle( "Reset", reset_button_color ) )
                view.reset();
            }
            ImGui::TreePop();
          }
          return change;
        }

        inline bool model( gtools::matrices::model& model, const char* const node_name )
        {
          bool change( false );
          if( ImGui::TreeNode( node_name ) )
          {
            {
              {
                change |= draging_button_xy( model.position, "XY", [&] ( auto& position, auto x, auto y ) { position.x += x * mouse_ratio; position.y -= y * mouse_ratio; } );
                ImGui::SameLine();
                change |= draging_button_x_or_y( model.position, "Z", [&] ( auto& position, auto z ) { position.z += z * mouse_ratio; } );
              }
              {
                change |= draging_button_x_or_y( model.rotation, "R(X)", [&] ( auto& rotation, auto x ) { rotation.x += x * mouse_ratio; } );
                ImGui::SameLine();
                change |= draging_button_x_or_y( model.rotation, "R(Y)", [&] ( auto& rotation, auto y ) { rotation.y += y * mouse_ratio; } );
                ImGui::SameLine();
                change |= draging_button_x_or_y( model.rotation, "R(Z)", [&] ( auto& rotation, auto z ) { rotation.z += z * mouse_ratio; } );
              }
              {
                change |= draging_button_x_or_y( model.scale, "Scale", [&] ( auto& scale, auto xyz ) { scale.x += xyz * mouse_ratio; scale.y += xyz * mouse_ratio; scale.z += xyz * mouse_ratio; } );
              }

              if( change )
                model.compute();
              if( ImGui::ButtonStyle( "Reset", reset_button_color ) )
                model.reset();
            }
            ImGui::TreePop();
          }
          return change;
        }

      }

      // ---- ---- ---- ----

    }
  }
}

