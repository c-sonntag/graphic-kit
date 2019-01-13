#pragma once

#include <gk/window/command/abstract.hpp>
#include <gk/window/abstract_controller.hpp>
#include <gk/render/painter/abstract.hpp>

#include <gk/math.hpp>

#include <glm/vec2.hpp>

#include <gk/debug.hpp>

namespace gk {
  namespace window {
    namespace command {

      struct mouse_lookat_center : public abstract
      {
       public:
        float mouse_speed = 0.25f;

       protected:
        bool cursor_reactivate = true;
        glm::vec2 last_cursor_position;

       public:
        using abstract::abstract;

       public:
        virtual void apply( render::painter::abstract& painter, const gk::time& t ) override
        {
          //
          // distance += controller.mouse_wheel().x;
          // distance += controller.mouse_wheel().y;

          //distance += controller.mouse_wheel().x;
          // debug_cout << "cursor_click(" << controller.cursor_click() << ")" << std::endl;

          const bool can_move(
            controller.is_clicked( window::mouse_button::left | window::mouse_button::right ) &&
            controller.have( m_modifier_required )
          );

          //
          if( !can_move )
          {
            if( !cursor_reactivate )
            {
              controller.active_cursor( true );
              cursor_reactivate = true;
            }
            return;
          }

          //
          const glm::vec2 cursor_position( controller.cursor_position() );
          gk::math::in_circle_property in_circle( painter.view.position.x, painter.view.position.z );

          //
          if( cursor_reactivate )
          {
            controller.active_cursor( false );
            last_cursor_position = cursor_position;
            cursor_reactivate = false;
          }

          //
          glm::vec2 diff( ( cursor_position - last_cursor_position ) * mouse_speed * t.seconds_deltatime );
          last_cursor_position = cursor_position;

          //
          if( controller.is_clicked( window::mouse_button::right ) )
          {
            if( in_circle.rayon > 0.1f )
              in_circle.rayon += diff.x + diff.y;
            diff.x = diff.y = 0.f;
          }


          painter.view.position.x = std::cos( in_circle.part + diff.x ) * in_circle.rayon;
          painter.view.position.z = std::sin( in_circle.part + diff.x ) * in_circle.rayon;
          painter.view.position.y += diff.y * in_circle.rayon;


          // //
          // painter.view.position.x = std::cos( angle + diff.x ) * distance;
          // painter.view.position.z = std::sin( angle + diff.y ) * distance;


          painter.view.compute();

        }
      };

    }
  }
}
