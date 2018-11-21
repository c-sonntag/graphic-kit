#pragma once

#include <gtk/types.hpp>
#include <gtk/window/abstract_receiver.hpp>
#include <gtk/window/command/abstract.hpp>

#include <gtk/render/painter_context.hpp>

namespace gtk {
  namespace window {

    struct abstract
    {
     protected:
      gtk::render::painter_context& context;
      abstract_receiver& receiver;

     protected:
      std::vector<std::unique_ptr<command::abstract>> commands;

     public:
      inline abstract( gtk::render::painter_context& _context ) :
        context( _context ),
        receiver( context.window_receiver )
      {}

     public:
      virtual ~abstract() = default;

     public:
      virtual void push_command( std::unique_ptr<command::abstract> command )
      { commands.emplace_back( std::move( command ) ); }

     public:
      virtual void run();

     public:
      virtual void poll() = 0; // at top in "run" loop
      virtual void clear() = 0; // at middle in "run" loop
      virtual void swap() = 0; // at bottom in "run" loop

     public:
      virtual bool check_close() = 0;

      // public:
      //  virtual void* handle() = 0;
      //  virtual const void* handle() const = 0;

    };


  }
}

// ---- ---- ---- ----

#include <gtk/window/abstract.ipp>
