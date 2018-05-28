#pragma once

#include <QObject>

namespace graphic_toolkit {

  class qt_object_signal_locker
  {
   private:
    QObject * const o_p;
    const bool init_state;
   public:
    qt_object_signal_locker( QObject * _o_p ) :
      o_p( std::move( _o_p ) ),
      init_state( o_p ? o_p->blockSignals( true ) : false )
    { }
    ~qt_object_signal_locker() {  if ( o_p ) o_p->blockSignals( init_state );}
  };

}
