#pragma once


namespace gtk {

  namespace render { struct painter_context; }

  namespace gui {

    class abstract
    {
     protected:
      friend render::painter_context;
      virtual void new_frame() = 0;
      virtual void render_frame() = 0;
      virtual void draw_render() = 0;

     public:
      /** marked as const ? */

    };

    // ---- ---- ---- ----

    template<typename TWindowsHandle>
    class abstract_handled : public abstract
    {
     public:
      TWindowsHandle* const window_handle;

     public:
      inline abstract_handled( TWindowsHandle* const _window_handle ) :
        window_handle( _window_handle ) {}

      virtual ~abstract_handled() = default;
    };


    //      using abstract_gui = gui::abstract<GLFWwindow>;
    //              std::unique_ptr<abstract_gui> gui,

  }
}
