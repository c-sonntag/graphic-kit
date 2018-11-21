#pragma once

#include <gtk/window/command/abstract.hpp>
#include <gtk/window/abstract_controller.hpp>
#include <gtk/render/painter/abstract.hpp>

#include <gtk/math.hpp>

#include <glm/vec2.hpp>

#include <gtk/debug.hpp>

namespace gtk {
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
        const key_modifier need_modifier;

       public:
        mouse_lookat_center( abstract_controller& _controller, const key_modifier _need_modifier ) :
          abstract( _controller ),
          need_modifier( std::move( _need_modifier ) )
        {}

       public:
        virtual void check( render::painter::abstract& painter, const gtk::time& t ) override
        {
          //
          // distance += controller.mouse_wheel().x;
          // distance += controller.mouse_wheel().y;

          //distance += controller.mouse_wheel().x;
          // debug_cout << "cursor_click(" << controller.cursor_click() << ")" << std::endl;

          const bool can_move(
            controller.is_clicked( window::mouse_button::left | window::mouse_button::right )
            && any( controller.key_modifier() & need_modifier )
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
          gtk::math::in_circle_property in_circle( painter.view.position.x, painter.view.position.z );

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
