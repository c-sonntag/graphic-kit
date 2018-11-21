#pragma once

#include <gtk/types.hpp>
#include <gtk/enum_bits.hpp>
#include <gtk/time.hpp>
#include <gtk/gui/abstract.hpp>

#include <gtk/matrices/view_lookat.hpp>
#include <gtk/matrices/base_camera.hpp>
#include <gtk/matrices/projection.hpp>
#include <glm/glm.hpp>

namespace gtk {
  namespace render {
    namespace painter {
      gtk_enum_bits( modes, uint32_t,
        paint = 1, anime = 2,
        paint_debug_gui = 4
      )
    }
  }
}

gtk_enum_bits_operator( gtk::render::painter::modes, uint32_t )

// ---- ---- ---- ----

namespace gtk {
  namespace render {
    namespace painter {

      struct abstract
      {
       public:
        gtk::render::painter::modes mode{ modes::_all };

       public:
        matrices::view_lookat view;
        matrices::base_camera camera;

       public:
        inline abstract( matrices::projection& _projection ) :
          camera( _projection, view ) {}

       public:
        virtual ~abstract() = default;

       public:
        virtual void paint() = 0;
        virtual void anime( const gtk::time& ) { /*nothing */ }
        virtual void paint_debug_gui( gtk::gui::abstract&  ) { /*nothing */ }
      };

    }
  }
}


