#pragma once

#include <gtools/render/abstract.hpp>

#include <gtools/render/painter/abstract.hpp>
#include <gtools/window/abstract_receiver.hpp>

#include <gtools/gui/abstract.hpp>

#include <gtools/frame_limiter.hpp>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>

namespace gtools { namespace window { struct abstract; }}

namespace gtools {
  namespace render {

    struct painter_context : public abstract
    {
     public:
      painter::modes painters_mode{ painter::modes::_all };

     public:
      gtools::frame_limiter fps_limiter{ 30 };

     protected:
      std::unique_ptr<gtools::gui::abstract> gui_up;

     private:
      std::vector<std::unique_ptr<painter::abstract>> painters;

     public:
      matrices::projection projection;

     protected:
      friend gtools::window::abstract;
      struct internal_window_receiver : public window::abstract_receiver
      {
       public:
        painter_context& context;
        inline internal_window_receiver( painter_context& _context ) :
          context( _context ) {}

       public:
        virtual void reshape( const glm::uvec2& resolution ) override
        {
          glViewport( 0, 0, GLint( resolution.x ), GLint( resolution.y ) );
          context.projection.aspect = float(resolution.x) / float(resolution.y > 0.f ? resolution.y : 1);
          context.projection.compute();
        }

      } window_receiver{ *this };

     public:
      inline void push_painter( std::unique_ptr<painter::abstract> p )
      { painters.emplace_back( std::move( p ) ); }

      inline void clear_painters()
      { painters.clear(); }

     public:
      inline void set_debug_gui( std::unique_ptr<gtools::gui::abstract> _gui_up )
      { gui_up = std::move( _gui_up ); }

     public:
      painter_context() = default;
      virtual ~painter_context() override = default;

     public:
      void prepare() override;
      void execute() override;
    };

  }
}

// ---- ---- ---- ----

#include <gtools/render/painter_context.ipp>

