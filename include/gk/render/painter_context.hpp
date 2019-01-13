#pragma once

#include <gk/render/abstract.hpp>

#include <gk/render/painter/abstract.hpp>
#include <gk/window/abstract_receiver.hpp>

#include <gk/gui/abstract.hpp>

#include <gk/frame_limiter.hpp>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>

namespace gk {

  namespace window { struct abstract; }

  namespace render {

    struct painter_context : public abstract
    {
     public:
      painter::modes painters_mode{ painter::modes::_all };

     public:
      gk::frame_limiter fps_limiter{ 30 };

     protected:
      std::unique_ptr<gk::gui::abstract> gui_up;

     private:
      std::vector<std::unique_ptr<painter::abstract>> painters;

     public:
      matrices::projection projection;

     protected:
      friend gk::window::abstract;
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
      /**
       * @brief push_painter : create unique_ptr painter of base instance gk::render::painter::abstract
       * @param args : arguments of commands
       */
      template<
        typename TPainter, typename ... Args,
        typename = std::enable_if_t<std::is_base_of<painter::abstract, TPainter>::value>
      >
      inline TPainter& push_painter( Args&& ... args )
      {
        painters.emplace_back( std::unique_ptr<TPainter>( new TPainter( args ... ) ) );
        // painters.emplace_back( std::make_unique<TPainter>( args ... ) );
        return *reinterpret_cast<TPainter*>( painters.back().get() );
      }

      inline void clear_painters()
      { painters.clear(); }

     public:
      inline void set_debug_gui( std::unique_ptr<gk::gui::abstract> _gui_up )
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

#include <gk/render/painter_context.ipp>

