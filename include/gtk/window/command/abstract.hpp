#pragma once

#include <gtk/window/abstract_controller.hpp>
#include <gtk/render/painter/abstract.hpp>

#include <glm/vec3.hpp>

namespace gtk {
  namespace window {
    namespace command {

      struct abstract
      {
       protected:
        render::painter::abstract& painter;

       public:
        abstract( render::painter::abstract& _painter ) :
          painter( _painter ) {}
        virtual ~abstract() = default;

       public:
        virtual void check( abstract_controller& controller, float seconds_deltatime ) = 0;

        // public:
        //  virtual void on_mouse_move( const glm::uvec2& pos ) = 0;
      };

    }
  }
}
