#include <gk/render/painter_context.hpp>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>

namespace gk {
  namespace render {


    // ---- ---- ---- ----

    inline void painter_context::prepare()
    {

      // ---- ----

      fps_limiter.cadence();
      const gk::time time( fps_limiter.time() );

      // ---- ----

      if( any( painters_mode & painter::modes::check_commands ) )
      {
        for( std::unique_ptr<painter::abstract>&painter : painters )
          if( any( painter->mode & painter::modes::check_commands ) )
            painter->apply_commands( time );
      }

      // ---- ----

      if( any( painters_mode & painter::modes::anime ) )
      {
        for( std::unique_ptr<painter::abstract>&painter : painters )
          if( any( painter->mode & painter::modes::anime ) )
            painter->anime( time );
      }

      // ---- ----

      if( gui_up )
        if( any( painters_mode & painter::modes::paint_debug_gui ) )
        {
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

