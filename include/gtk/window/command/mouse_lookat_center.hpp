#pragma once

#include <gtk/window/command/abstract.hpp>

#include <glm/vec3.hpp>

namespace gtk {
  namespace window {
    namespace command {

      struct mouse_lookat_center : public abstract
      {
       public:
        using abstract::abstract;
        virtual void check( abstract_controller& controller, float seconds_deltatime )
        {


        }
      };

    }
  }
}
