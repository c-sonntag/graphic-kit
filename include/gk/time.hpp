#pragma once

#include <bits/move.h>

namespace gk {

  struct time
  {
    float seconds_elpased;
    float seconds_deltatime;

    inline time( const float _seconds_elpased, const float _seconds_deltatime ) :
      seconds_elpased( _seconds_elpased ),
      seconds_deltatime( _seconds_deltatime )
    {}

  };

}
