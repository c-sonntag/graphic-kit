#pragma once

#include <gtk/types.hpp>
#include <gtk/window/abstract_receiver.hpp>
#include <gtk/window/abstract_controller.hpp>

#include <gtk/render/painter_context.hpp>

#include <type_traits>
#include <vector>
#include <memory>

namespace gtk {
  namespace window {

    struct abstract
    {
     protected:
      gtk::render::painter_context& context;
      abstract_receiver& receiver;

     public:
      inline abstract( gtk::render::painter_context& _context ) :
        context( _context ),
        receiver( context.window_receiver )
      {}

     public:
      virtual ~abstract() = default;

     public:
      virtual void run();

     public:
      virtual void poll() = 0; // at top in "run" loop
      virtual void clear() = 0; // at middle in "run" loop
      virtual void swap() = 0; // at bottom in "run" loop

    public:
      virtual abstract_controller & controller() = 0;

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
