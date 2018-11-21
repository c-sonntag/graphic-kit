#pragma once

#include <gtk/time.hpp>

#include <glm/vec3.hpp>

namespace gtk {

  namespace render {
    namespace painter {
      struct abstract;
    }
  }

  namespace window {

    struct abstract_controller;

    // ---- ----

    namespace command {

      struct abstract
      {
       protected:
        abstract_controller& controller;

       public:
        abstract( abstract_controller& _controller ) :
          controller( _controller ) {}

        virtual ~abstract() = default;

       public:
        virtual void check( render::painter::abstract& painter, const gtk::time& t ) = 0;

        // public:
        //  virtual void on_mouse_move( const glm::uvec2& pos ) = 0;
      };

    }
  }
}
