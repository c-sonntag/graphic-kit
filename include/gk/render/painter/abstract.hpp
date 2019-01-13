#pragma once

#include <gk/types.hpp>
#include <gk/enum_bits.hpp>
#include <gk/time.hpp>

#include <gk/window/abstract_controller.hpp>
#include <gk/window/command/abstract.hpp>
#include <gk/gui/abstract.hpp>

#include <gk/matrices/view_lookat.hpp>
#include <gk/matrices/base_camera.hpp>
#include <gk/matrices/projection.hpp>

#include <glm/glm.hpp>

namespace gk {
  namespace render {
    namespace painter {
      gk_enum_bits( modes, uint32_t,
        paint = 1, anime = 2,
        paint_debug_gui = 4,
        check_commands = 8
      )
    }
  }
}

gk_enum_bits_operator( gk::render::painter::modes, uint32_t )

// ---- ---- ---- ----

namespace gk {
  namespace render {
    namespace painter {

      struct abstract
      {
       public:
        gk::render::painter::modes mode{ modes::_all };

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
        virtual void anime( const gk::time& ) { /*nothing */ }
        virtual void paint_debug_gui( gk::gui::abstract&  ) { /*nothing */ }

       protected:
        std::vector<std::unique_ptr<window::command::abstract>> commands;

       public:
        inline void apply_commands( const gk::time& t )
        {
          for( std::unique_ptr<window::command::abstract>& command_up : commands )
            if( command_up )
              command_up->apply( *this, t );
        }

       public:
        /**
         * @brief push_command : create unique_ptr command of base instance gk::window::command::abstract
         * @param args : arguments of commands
         */
        template<typename TCommand, typename ... Args, typename = std::enable_if_t<std::is_base_of<window::command::abstract, TCommand>::value>>
        inline TCommand& push_command( gk::window::abstract_controller& controller, Args&& ... args )
        {
          commands.emplace_back( std::unique_ptr<TCommand>( new TCommand( controller, args ... ) ) );
          return *reinterpret_cast<TCommand*>( commands.back().get() );
        }

        inline void clear_commands() { commands.clear(); }


      };

    }
  }
}


