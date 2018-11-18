#include <gtools/window/abstract.hpp>

namespace gtools {
  namespace window {

    inline void abstract::run()
    {

      //
      bool run = true;

      //
      while( run )
      {
        //
        poll();

        //
        context.prepare();

        //
        clear();

        //
        context.execute();

        //
        swap();

        //

        if( check_close() )
          run = false;
      }


    }


  }
}
