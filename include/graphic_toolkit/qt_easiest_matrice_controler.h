#ifndef QT_EASIEST_MATRICE_CONTROLER_H
#define QT_EASIEST_MATRICE_CONTROLER_H

#include <QWidget>
#include <QEvent>
#include <QOpenGLWidget>

#include <QTime>
#include <QBasicTimer>

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>

#include <memory>

namespace Ui {
  class qt_easiest_matrice_controler;
}

namespace graphic_toolkit {

  class qt_easiest_matrice_controler : public QWidget
  {
    Q_OBJECT

   public:
    enum class ProjectionMode { Ortho, Perspective };
    enum class DisplacementMode { FreeView, FreeCamera, FreeView_TimerRotation };

   public:
    //struct slots_receiver
    //{
    //  virtual void set_viewOrtho() = 0;
    //  virtual void set_viewPerspective() = 0;
    //  virtual void set_fov( float value ) = 0;
    //  virtual void set_zoom( float value ) = 0;
    //};

   public:
    explicit qt_easiest_matrice_controler( QWidget * parent = nullptr );
    virtual ~qt_easiest_matrice_controler() override;

   protected:
    QTime chrono;

   protected:
    struct locker_repaint_glcanvas
    {
     protected:
      qt_easiest_matrice_controler & emc;
      bool can_unlock;
     public:
      locker_repaint_glcanvas( qt_easiest_matrice_controler * const emc );
      ~locker_repaint_glcanvas();
    };

   protected:
    bool repaint_glcanvas_wait_end = false;

   public:
    void init_after_gl( QOpenGLWidget * _glwidget );
    void repaint_glcanvas( bool force = false );
    void refresh_form();

   private slots:
    void on_radioButton_viewOrtho_clicked();
    void on_radioButton_viewPerspective_clicked();

    void on_doubleSpinBox_fov_valueChanged( double value );
    void on_doubleSpinBox_zoom_valueChanged( double value );

    void on_radioButton_freeView_clicked();
    void on_radioButton_freeCamera_clicked();
    void on_radioButton_freeView_timerRotation_clicked();

    void on_pushButton_resetView_clicked();


   protected:
    static constexpr float camera_move_speed = 0.10f;
    static constexpr float camera_angle_speed = 0.001f;
    static constexpr float view_rotation_speed = 2.f;
    static constexpr float view_move_speed = 0.01f;
    static constexpr float view_angle_rotation_speed = 0.1f;

   protected:
    static constexpr float correction_zoom_ortho = -10.f;
    static constexpr float correction_zoom_perspective = -10.f;

   protected:
    float default_z = 8.f;
    float default_zoom = 1.f;
    float default_fov = 45.f;

   public:
    inline void change_default_z( float _value ) {default_z = _value;}
    inline void change_default_zoom( float _value ) {default_zoom = _value;}
    inline void change_default_fov( float _value ) {default_fov = _value;}

   protected:
    float view_angle_rotation_mult = 0.f;
    QVector2D view_angle_rotation_axe;
    QVector2D camera_angle;
    QVector3D camera_right;
    QVector3D camera_up;
    QVector3D camera_position;
    QVector3D camera_direction;
    QVector3D view_angle;
    float zoom;
    float fov;

   public:
    void reset_view();

   protected:
    void compute_camera();

   protected:
    ProjectionMode projection_mode = ProjectionMode::Perspective;
    float current_aspect = 4.f / 3.f;

   protected:
    QMatrix4x4 view;
    QMatrix4x4 projection;
    QMatrix4x4 view_projection;

   protected:
    inline void compute_view_projection() {view_projection = projection * view;}
    inline float smooth_zoom();

   protected:
    void compute_view();
    void compute_projection();

   public:
    inline const QMatrix4x4 & get_view_projection() const {
      return view_projection;
    }

   protected:
    bool inverted_rotation_move_touch = false;

   protected:
    bool key_ctrl_down = false;
    bool key_shift_down = false;
    QPoint mouse_last_position;
    QPoint mouse_click_position;
    bool mouse_tracking = false;
    QVector2D mouse_move_cumul;
    QVector2D mouse_once_direction;

   public:
    void receive_resize( int w, int h );
    void receive_mousePressEvent( QMouseEvent * e );
    void receive_mouseReleaseEvent( QMouseEvent * e );
    void receive_mouseMoveEvent( QMouseEvent * e );
    void receive_wheelEvent( QWheelEvent * e );

   protected:
    bool eventFilter( QObject * object, QEvent * event ) override;

   protected:
    QBasicTimer timer;
    void timer_start();
    void timer_stop();
    void timerEvent( QTimerEvent * e ) override;

   protected:
    bool want_rotation() const;

   protected:
    DisplacementMode displacement_mode = DisplacementMode::FreeView;
    void compute_mouseMove_freeView( const QMouseEvent * e, const QVector2D & diff );
    void compute_mouseMove_freeCamera( const QMouseEvent * e, const QVector2D & diff );
    void compute_mouseMove_freeView_and_timerRotation( const QMouseEvent * e, const QVector2D & diff );
    void compute_mouseRelease_freeView_and_timerRotation( const QMouseEvent * e, const QVector2D & diff );
    void compute_timer_freeView_and_timerRotation( const QTimerEvent * e );
    bool compute_keyboard_freeView( const QKeyEvent * e );
    bool compute_keyboard_freeCamera( const QKeyEvent * e );

   public:
    void change_displacement( DisplacementMode mode );
    void change_displacement_only( DisplacementMode mode );

   public:
    void change_displacement_inverted_rotation_move_touch( bool value );

   public:
    void change_projection( ProjectionMode mode );
    void set_fov( float value );
    void set_zoom( float value );

   protected:
    void change_projection_only( ProjectionMode mode );
    void set_fov_only( float value );
    void set_zoom_only( float value );

   private:
    QOpenGLWidget * glwidget = nullptr;
    Ui::qt_easiest_matrice_controler * const ui;

    //private:
    //std::shared_ptr<slots_receiver> receiver_sp;
  };

}

#endif // QT_EASIEST_MATRICE_CONTROLER_H
