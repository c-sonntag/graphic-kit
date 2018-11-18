#pragma once

namespace gtools {
  namespace window {
    namespace gui {

      class abstract
      {
       public:
        virtual void gui() = 0;
      };

      // ---- ---- ---- ----

      template<typename TWindowsHandle>
      class abstract_handled : public abstract
      {
       public:
        TWindowsHandle* const window_handle;

       public:
        inline abstract_handled( TWindowsHandle * const _window_handle ) :
          window_handle( _window_handle ) {}

        virtual ~abstract() = default;
      };


      //      using abstract_gui = gui::abstract<GLFWwindow>;
      //              std::unique_ptr<abstract_gui> gui,

    }
  }
}
