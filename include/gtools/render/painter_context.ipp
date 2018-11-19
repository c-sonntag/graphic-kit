#include <gtools/render/painter_context.hpp>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>

namespace gtools {
  namespace render {


    // ---- ---- ---- ----

    inline void painter_context::prepare()
    {


      // ---- ----

      fps_limiter.cadence();

      // ---- ----

      if( any( painters_mode & painter::modes::anime ) )
      {
        const gtools::time time( fps_limiter.time() );
        for( std::unique_ptr<painter::abstract>&painter : painters )
          if( any( painter->mode & painter::modes::anime ) )
            painter->anime( time );
      }

      // ---- ----

      if( gui_up )
        if( any( painters_mode & painter::modes::paint_debug_gui ) )
        {
          // Start the Dear ImGui frame
          gui_up->new_frame();

          for( std::unique_ptr<painter::abstract>&painter : painters )
            if( any( painter->mode & painter::modes::paint_debug_gui ) )
              painter->paint_debug_gui( *gui_up );

          gui_up->render_frame();
        }

      // ---- ----

    }

    inline void painter_context::execute()
    {

      // ---- ----

      if( any( painters_mode & painter::modes::paint ) )
        for( std::unique_ptr<painter::abstract>&painter : painters )
          if( any( painter->mode & painter::modes::paint ) )
            painter->paint();

      // ---- ----

      if( gui_up )
        if( any( painters_mode & painter::modes::paint_debug_gui ) )
          gui_up->draw_render();

      // ---- ----

    }


  }
}

