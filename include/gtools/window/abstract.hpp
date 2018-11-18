#pragma once

#include <gtools/types.hpp>
#include <gtools/window/abstract_receiver.hpp>

#include <gtools/render/painter_context.hpp>

namespace gtools {
  namespace window {

    struct abstract
    {
     public:
      gtools::render::painter_context& context;
      abstract_receiver& receiver;

     public:
      inline abstract( gtools::render::painter_context& _context ) :
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
      virtual bool check_close() = 0;

      // public:
      //  virtual void* handle() = 0;
      //  virtual const void* handle() const = 0;

    };


  }
}

// ---- ---- ---- ----

#include <gtools/window/abstract.ipp>
