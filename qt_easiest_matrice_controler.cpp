#include <graphic_toolkit/qt_easiest_matrice_controler.h>
#include "ui_qt_easiest_matrice_controler.h"

#include <graphic_toolkit/constants.h>

#include <QMouseEvent>
#include <QCoreApplication>

#include <cmath>

// ---- ---- ---- ----

using namespace graphic_toolkit::constants;

graphic_toolkit::qt_easiest_matrice_controler::qt_easiest_matrice_controler( QWidget * parent ) :
  QWidget( parent ),
  ui( new Ui::qt_easiest_matrice_controler() )
{
  ui->setupUi( this );

  //
  QCoreApplication::instance()->installEventFilter( this );

  //
  reset_view();
  refresh_form();
}

graphic_toolkit::qt_easiest_matrice_controler::~qt_easiest_matrice_controler()
{
  delete ui;
}

// ---- ---- ---- ----

inline graphic_toolkit::qt_easiest_matrice_controler::locker_repaint_glcanvas::locker_repaint_glcanvas( qt_easiest_matrice_controler * const _emc_p ) :
  emc( *_emc_p ), can_unlock( !emc.repaint_glcanvas_wait_end )
{
  if ( can_unlock )
    emc.repaint_glcanvas_wait_end = true;
}

inline graphic_toolkit::qt_easiest_matrice_controler::locker_repaint_glcanvas::~locker_repaint_glcanvas()
{
  if ( can_unlock )
  {
    emc.repaint_glcanvas_wait_end = false;
    emc.repaint_glcanvas( true );
  }
}


// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::init_after_gl( QOpenGLWidget * _glwidget )
{
  glwidget = _glwidget;

  // For repaint_glcanvas
  chrono.start();
}

void graphic_toolkit::qt_easiest_matrice_controler::repaint_glcanvas( bool force )
{
  if ( ( ( chrono.elapsed() > 5 ) || force ) && !repaint_glcanvas_wait_end )
  {
    if ( glwidget )
    {
      chrono.restart();
      glwidget->repaint();
    }
  }
}

void graphic_toolkit::qt_easiest_matrice_controler::refresh_form()
{
  locker_repaint_glcanvas locker( this );

  //
  change_projection( projection_mode );
  change_displacement( displacement_mode );

  //
  set_zoom( zoom );
  set_fov( fov );
}

// ---- ---- ---- ----


void graphic_toolkit::qt_easiest_matrice_controler::on_radioButton_viewOrtho_clicked()
{
  change_projection_only( ProjectionMode::Ortho );
  ui->doubleSpinBox_fov->setEnabled( false );
  update();
}

void graphic_toolkit::qt_easiest_matrice_controler::on_radioButton_viewPerspective_clicked()
{
  change_projection_only( ProjectionMode::Perspective );
  ui->doubleSpinBox_fov->setEnabled( true );
  update();
}

void graphic_toolkit::qt_easiest_matrice_controler::on_doubleSpinBox_fov_valueChanged( double value )
{
  set_fov_only( float( value ) );
}

void graphic_toolkit::qt_easiest_matrice_controler::on_doubleSpinBox_zoom_valueChanged( double value )
{
  set_zoom_only( float( value ) );
}

void graphic_toolkit::qt_easiest_matrice_controler::on_radioButton_freeView_clicked()
{
  change_displacement_only( DisplacementMode::FreeView );
}

void graphic_toolkit::qt_easiest_matrice_controler::on_radioButton_freeCamera_clicked()
{
  change_displacement_only( DisplacementMode::FreeCamera );
}

void graphic_toolkit::qt_easiest_matrice_controler::on_radioButton_freeView_timerRotation_clicked()
{
  change_displacement_only( DisplacementMode::FreeView_TimerRotation );
}

void graphic_toolkit::qt_easiest_matrice_controler::on_pushButton_resetView_clicked()
{
  reset_view();
}


// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::reset_view()
{
  //camera_angle = QVector2D( 0.f, 0.f );
  camera_angle = QVector2D( pi, 0.f );
  camera_position = QVector3D( 0.f, 0.f, 0.f );
  view_angle = QVector3D( 0.f, 0.f, 0.f );
  //
  set_zoom( default_zoom );
  set_fov( default_fov );
  //
  compute_camera();
  compute_view();
  repaint_glcanvas();
}

void graphic_toolkit::qt_easiest_matrice_controler::compute_camera()
{
  camera_direction = QVector3D(
                       std::cos( camera_angle.y() ) * std::sin( camera_angle.x() ),
                       std::sin( camera_angle.y() ),
                       std::cos( camera_angle.y() ) * std::cos( camera_angle.x() )
                     );

  camera_right = QVector3D(
                   std::sin( camera_angle.x() - pi / 2.0f ),
                   0,
                   std::cos( camera_angle.x() - pi / 2.0f )
                 );

  camera_up = QVector3D( QVector3D::crossProduct( camera_right, camera_direction ) );
}

// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::compute_view()
{
  //
  QVector3D adapted_position( camera_position );
  if ( projection_mode == ProjectionMode::Ortho )
    adapted_position.setZ( correction_zoom_ortho );

  //
  view.setToIdentity();

  //
  view.lookAt(
    camera_position,
    camera_position + camera_direction,
    camera_up
  );

  //
  view.translate( camera_direction * zoom );

  //
  view.rotate( view_angle.x() / 16.0f, 1.f, 0, 0 );
  view.rotate( view_angle.y() / 16.0f, 0, 1.f, 0 );
  view.rotate( view_angle.z() / 16.0f, 0, 0, 1.f );

  //
  compute_view_projection();
  repaint_glcanvas();
}


void graphic_toolkit::qt_easiest_matrice_controler::compute_projection()
{
  static constexpr float zNear( 0.001f ), zFar( 1000.f );

  // Reset projection
  projection.setToIdentity();

  // Set ortho projection
  if ( projection_mode == ProjectionMode::Ortho )
    projection.ortho(
      -zoom * current_aspect, zoom * current_aspect,
      -zoom, zoom,
      zNear, zFar
    );

  // Set perspective projection
  else if ( projection_mode == ProjectionMode::Perspective )
    projection.perspective( fov, current_aspect, zNear, zFar );

  //
  compute_view_projection();
  repaint_glcanvas();
}


// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::receive_resize( int w, int h )
{
  //
  const float aspect = float( w ) / float( h ? h : 1 );
  current_aspect = aspect;

  //
  compute_projection();
}

void graphic_toolkit::qt_easiest_matrice_controler::receive_mousePressEvent( QMouseEvent * e )
{
  mouse_click_position = e->pos();
  mouse_last_position = mouse_click_position;
  mouse_move_cumul = QVector2D( 0.f, 0.f );

  if ( !mouse_tracking )
  {
    QCursor c = cursor();
    c.setShape( Qt::BlankCursor );
    QApplication::setOverrideCursor( c );
    mouse_tracking = true;
  }
}

void graphic_toolkit::qt_easiest_matrice_controler::receive_mouseReleaseEvent( QMouseEvent * e )
{
  if ( mouse_tracking )
  {
    QCursor c = cursor();
    c.setShape( Qt::ArrowCursor );
    QApplication::setOverrideCursor( c );
    mouse_tracking = false;
  }
  if ( !mouse_once_direction.isNull() )
    mouse_once_direction = QVector2D( 0.f, 0.f );

  if ( displacement_mode == DisplacementMode::FreeView_TimerRotation )
    compute_mouseRelease_freeView_and_timerRotation( e, mouse_move_cumul );

}

void graphic_toolkit::qt_easiest_matrice_controler::receive_mouseMoveEvent( QMouseEvent * e )
{
  //
  if ( !( e->buttons() & Qt::RightButton ) && !( e->buttons() & Qt::LeftButton ) )
    return;

  //
  if ( mouse_tracking )
    if ( mouse_click_position == e->pos() )
      return;

  //
  QVector2D diff( e->pos() - ( mouse_tracking ? mouse_click_position : mouse_last_position ) );
  mouse_last_position = e->pos();
  mouse_move_cumul += diff;

  //
  if ( key_shift_down && mouse_once_direction.isNull() )
    mouse_once_direction = QVector2D(
                             ( diff.x() > diff.y() ) ? 1.f : 0.f,
                             ( diff.x() > diff.y() ) ? 0.f : 1.f
                           );

  //
  if ( mouse_tracking && glwidget )
  {
    QCursor * const c( QApplication::overrideCursor() );
    c->setPos( glwidget->mapToGlobal( mouse_click_position ) );
  }

  // effect
  if ( !mouse_once_direction.isNull() )
    diff *= mouse_once_direction;

  //
  if ( displacement_mode == DisplacementMode::FreeView )
    compute_mouseMove_freeView( e, diff );
  else if ( displacement_mode == DisplacementMode::FreeCamera )
    compute_mouseMove_freeCamera( e, diff );
  else if ( displacement_mode == DisplacementMode::FreeView_TimerRotation )
    compute_mouseMove_freeView_and_timerRotation( e, diff );

}

void graphic_toolkit::qt_easiest_matrice_controler::receive_wheelEvent( QWheelEvent * e )
{
  const float movCorrection { float( e->delta() ) / 120.f };
  //
  if ( key_ctrl_down )
    set_fov( fov + movCorrection );
  else
    set_zoom( zoom + ( movCorrection / 10.f ) );
}


// ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::timer_start()
{
  // Use QBasicTimer because its faster than QTimer
  if ( !timer.isActive() )
    timer.start( 12, this );
}

void graphic_toolkit::qt_easiest_matrice_controler::timer_stop()
{
  timer.stop();
}

void graphic_toolkit::qt_easiest_matrice_controler::timerEvent( QTimerEvent * e )
{
  //
  if ( displacement_mode == DisplacementMode::FreeView_TimerRotation )
    compute_timer_freeView_and_timerRotation( e );
  else
    view_angle_rotation_mult = 0.0f;
}

// ---- ----

bool graphic_toolkit::qt_easiest_matrice_controler::eventFilter( QObject * o, QEvent * event )
{
  if ( !( event->type() == QEvent::KeyPress ) && !( event->type() == QEvent::KeyRelease ) )
    return QObject::eventFilter( o, event );

  //
  QKeyEvent * key_event = dynamic_cast<QKeyEvent *>( event );

  if ( event->type() == QEvent::KeyPress )
  {
    //
    if ( key_event->key() == Qt::Key_L )
    {reset_view(); return true;}

    else if ( key_event->key() == Qt::Key_Control && !key_ctrl_down )
    {key_ctrl_down = true; return true;}

    else if ( key_event->key() == Qt::Key_Shift && !key_shift_down )
    {key_shift_down = true; return true;}

    else if ( displacement_mode == DisplacementMode::FreeView )
      return compute_keyboard_freeView( key_event );

    else if ( displacement_mode == DisplacementMode::FreeCamera )
      return compute_keyboard_freeCamera( key_event );

    return false;
  }
  else if ( event->type() == QEvent::KeyRelease )
  {
    if ( key_event->key() == Qt::Key_Control && key_ctrl_down )
    {key_ctrl_down = false; return true;}

    else if ( key_event->key() == Qt::Key_Shift && key_shift_down )
    {key_shift_down = false; return true;}

    return false;
  }

  //
  return QObject::eventFilter( o, event );
}

// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::compute_mouseMove_freeView( const QMouseEvent * e, const QVector2D & diff )
{
  if ( key_ctrl_down )
  {
    //
    const QVector2D force( diff * view_rotation_speed );

    //
    if ( e->buttons() & Qt::LeftButton )
      view_angle += QVector3D( force.y(), force.x(), 0.f );
    else if ( e->buttons() & Qt::RightButton )
      view_angle += QVector3D( force.y(), 0.f, force.x() );

    //
    compute_view();
  }
  else
  {
    //
    const QVector2D force( diff * view_move_speed );

    //
    camera_position += camera_right * -force.x();

    //
    if ( e->buttons() & Qt::LeftButton )
      camera_position += camera_up * force.y();
    else if ( e->buttons() & Qt::RightButton )
      camera_position += camera_direction * force.y();

    //
    compute_view();
  }
}

void graphic_toolkit::qt_easiest_matrice_controler::compute_mouseMove_freeCamera( const QMouseEvent * e, const QVector2D & diff )
{
  if ( key_ctrl_down )
    compute_mouseMove_freeView( e, diff );

  else if ( e->buttons() & Qt::LeftButton )
  {
    camera_angle += diff * camera_angle_speed;
    compute_camera();
    compute_view();
  }
}

void graphic_toolkit::qt_easiest_matrice_controler::compute_mouseMove_freeView_and_timerRotation( const QMouseEvent * e, const QVector2D & diff )
{
  if ( !key_ctrl_down )
    compute_mouseMove_freeView( e, diff );
}

void graphic_toolkit::qt_easiest_matrice_controler::compute_mouseRelease_freeView_and_timerRotation( const QMouseEvent *, const QVector2D & diff )
{
  if ( key_ctrl_down )
  {
    const QVector2D diff_correction( -diff.x(), diff.y() );

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    const QVector2D n = QVector2D( diff_correction.y(), diff_correction.x() ).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    const float acc( diff_correction.length() * view_angle_rotation_speed );

    // Calculate new rotation axis as weighted sum
    view_angle_rotation_axe = ( view_angle_rotation_axe * view_angle_rotation_mult + n * acc ).normalized();

    // Increase angular speed
    view_angle_rotation_mult += acc;

    //
    timer_start();
  }
}

void graphic_toolkit::qt_easiest_matrice_controler::compute_timer_freeView_and_timerRotation( const QTimerEvent * )
{


  //
  if ( key_ctrl_down )
  {
    // Decrease angular speed (friction)
    view_angle_rotation_mult *= 0.99f;
    // Stop rotation when speed goes below threshold
    if ( view_angle_rotation_mult < 0.01f )
    {
      view_angle_rotation_mult = 0.0f;
      timer_stop();
    }
    else
    {
      // Update rotation
      view_angle += view_angle_rotation_axe * view_angle_rotation_mult;
      compute_view();
    }
  }
  else
  {
    view_angle_rotation_mult = 0.0f;
    timer_stop();
  }
}

// ---- ----

bool graphic_toolkit::qt_easiest_matrice_controler::compute_keyboard_freeView( const QKeyEvent * )
{
  return false;
}

bool graphic_toolkit::qt_easiest_matrice_controler::compute_keyboard_freeCamera( const QKeyEvent * e )
{
  //if ( !e->isAutoRepeat() )
  //  return false;

  switch ( e->key() )
  {
  case Qt::Key_Up:
    camera_position += camera_direction * camera_move_speed;
    break;
  case Qt::Key_Down:
    camera_position -= camera_direction * camera_move_speed;
    break;
  case Qt::Key_Right:
    camera_position += camera_right * camera_move_speed;
    break;
  case Qt::Key_Left:
    camera_position -= camera_right * camera_move_speed;
    break;
  default:
    return false;
  }
  compute_view();
  return true;
}

// ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::change_displacement( DisplacementMode mode )
{
  change_displacement_only( mode );
  if ( mode == DisplacementMode::FreeView )
    ui->radioButton_freeView->click();
  else if ( mode == DisplacementMode::FreeCamera )
    ui->radioButton_freeCamera->click();
  else if ( mode == DisplacementMode::FreeView_TimerRotation )
    ui->radioButton_freeView_timerRotation->click();
}

void graphic_toolkit::qt_easiest_matrice_controler::change_displacement_only( DisplacementMode mode )
{
  displacement_mode = mode;
  compute_view();
}



// ---- ---- ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::change_projection( ProjectionMode mode )
{
  change_projection_only( mode );
  if ( mode == ProjectionMode::Ortho )
    ui->radioButton_viewOrtho->click();
  else if ( mode == ProjectionMode::Perspective )
    ui->radioButton_viewPerspective->click();
}


void graphic_toolkit::qt_easiest_matrice_controler::set_fov( float value )
{
  set_fov_only( value );
  ui->doubleSpinBox_fov->setValue( double( fov ) );
}

void graphic_toolkit::qt_easiest_matrice_controler::set_zoom( float value )
{
  set_zoom_only( value );
  ui->doubleSpinBox_zoom->setValue( double( zoom ) );
}

// ---- ----

void graphic_toolkit::qt_easiest_matrice_controler::change_projection_only( ProjectionMode mode )
{
  locker_repaint_glcanvas locker( this );

  //
  projection_mode = mode;
  compute_view();
  compute_projection();
}

void graphic_toolkit::qt_easiest_matrice_controler::set_fov_only( float value )
{
  locker_repaint_glcanvas locker( this );

  fov = value;
  compute_projection();
}

void graphic_toolkit::qt_easiest_matrice_controler::set_zoom_only( float value )
{
  locker_repaint_glcanvas locker( this );

  zoom = value;
  compute_view();
  if ( projection_mode == ProjectionMode::Ortho )
    compute_projection();
}




