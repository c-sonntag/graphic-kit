#pragma once

#include <gk/types.hpp>
#include <gk/window/abstract_receiver.hpp>
#include <gk/window/abstract_controller.hpp>

#include <gk/render/painter_context.hpp>

#include <type_traits>
#include <vector>
#include <memory>

namespace gk {
  namespace window {

    struct abstract
    {
     protected:
      gk::render::painter_context& context;
      abstract_receiver& receiver;

     public:
      inline abstract( gk::render::painter_context& _context ) :
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

#include <gk/window/abstract.ipp>
