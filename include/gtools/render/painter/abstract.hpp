#pragma once

#include <gtools/types.hpp>
#include <gtools/enum_bits.hpp>
#include <gtools/time.hpp>
#include <gtools/gui/abstract.hpp>

#include <gtools/matrices/view_lookat.hpp>
#include <gtools/matrices/base_camera.hpp>
#include <gtools/matrices/projection.hpp>
#include <glm/glm.hpp>

namespace gtools {
  namespace render {
    namespace painter {
      ADD_ENUM_BITS ( modes, uint32_t,
        paint = 1, anime = 2,
        paint_debug_gui = 4
      )
    }
  }
}

ADD_ENUM_OPERATOR( gtools::render::painter::modes, uint32_t )

// ---- ---- ---- ----

namespace gtools {
  namespace render {
    namespace painter {

      struct abstract
      {
       public:
        gtools::render::painter::modes mode{ modes::_all };

       protected:
        matrices::view_lookat view;
        matrices::base_camera camera;

       public:
        inline abstract( matrices::projection& _projection ) :
          camera( _projection, view ) {}

       public:
        virtual ~abstract() = default;

       public:
        virtual void paint() = 0;
        virtual void anime( const gtools::time& ) { /*nothing */ }
        virtual void paint_debug_gui( gtools::gui::abstract&  ) { /*nothing */ }
      };

    }
  }
}


