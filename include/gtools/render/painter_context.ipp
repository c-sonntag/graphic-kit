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
      // //
      // bool run = true;
      //
      // //
      // while( run )
      // {

      // window.poll();
      fps_limiter.cadence();


      // ---- ----

      if( any( painters_mode & painter::modes::anime ) )
      {
        const gtools::time time( fps_limiter.time() );
        for( std::unique_ptr<painter::abstract>&painter : painters )
          painter->anime( time );
      }

      // ---- ----

      //  if( any( painters_mode & painter::modes::paint_debug_gui ) )
      //  {
      //    // Start the Dear ImGui frame
      //    ImGui_ImplOpenGL3_NewFrame();
      //    ImGui_ImplGlfw_NewFrame();
      //    ImGui::NewFrame();
      //
      //    for( std::unique_ptr<painter::abstract>&painter : painters )
      //      painter->paint_gui_debug();
      //
      //    ImGui::Render();
      //  }

      // ---- ----

    }

    inline void painter_context::execute()
    {

      for( std::unique_ptr<painter::abstract>&painter : painters )
        painter->paint();

      // ---- ----

      // ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

      // ---- ----

      // window.swap();

      // ---- ----

    }


  }
}

