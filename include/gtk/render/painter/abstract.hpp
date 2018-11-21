#pragma once

#include <gtk/types.hpp>
#include <gtk/enum_bits.hpp>
#include <gtk/time.hpp>

#include <gtk/window/abstract_controller.hpp>
#include <gtk/window/command/abstract.hpp>
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
        paint_debug_gui = 4,
        check_commands = 8
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

       protected:
        std::vector<std::unique_ptr<window::command::abstract>> commands;

       public:
        virtual void check_commands( const gtk::time& t )
        {
          for( std::unique_ptr<window::command::abstract>& command_up : commands )
            if( command_up )
              command_up->check( *this, t );
        }

       public:
        /**
         * @brief push_command : create unique_ptr command of base instance gtk::window::command::abstract
         * @param args : arguments of commands
         */
        template<
          typename TCommand, typename ... Args,
          typename = std::enable_if_t<std::is_base_of<window::command::abstract, TCommand>::value>
        >
        inline TCommand& push_command( gtk::window::abstract_controller& controller, Args&& ... args )
        {
          commands.emplace_back( std::unique_ptr<TCommand>( new TCommand( controller, args ... ) ) );
          return *reinterpret_cast<TCommand*>( commands.back().get() );
        }

        inline void clear_commands() { commands.clear(); }


      };

    }
  }
}


