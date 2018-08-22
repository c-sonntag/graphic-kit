#pragma once

#include <graphic_toolkit/types.h>

#include <chrono>
#include <thread>

namespace graphic_toolkit {

  struct frame_limiter
  {
   public:
    using internal_clock = std::chrono::system_clock;

   private:
    uint fps_limit;
    int64_t microseconds_limit;

   private:
    internal_clock::time_point last;
    internal_clock::time_point current;

   private:
    int64_t elapsed_milliseconds = 0;
    int64_t elapsed_microseconds = 0;

   public:
    inline frame_limiter( const uint _fps_limit = 33 ) :
      fps_limit( std::move( _fps_limit ) ),
      last( internal_clock::now() )
    { compute_microseconds_limit(); }

    inline void set_limit( const uint _fps_limit )
    { fps_limit = _fps_limit; compute_microseconds_limit();}

   private:
    inline void compute_microseconds_limit()
    { microseconds_limit = static_cast<int64_t>( 1. / static_cast<double>( fps_limit ) * 1'000'000. ); }

   public:
    inline void cadence() {

      //
      current = internal_clock::now();
      elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds> ( current - last ).count();
      elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds> ( current - last ).count();
      last = current;

      //
      if ( elapsed_microseconds < microseconds_limit )
        std::this_thread::sleep_for( std::chrono::microseconds( microseconds_limit - elapsed_microseconds ) );
    }

   public:
    inline int64_t current_elapsed_milliseconds() const { return elapsed_milliseconds; }
    inline int64_t current_elapsed_microseconds() const { return elapsed_microseconds; }

    inline uint current_fps() const { return static_cast<uint>( 1. / ( static_cast<double>( elapsed_microseconds ) * 0.000'001 ) ); }

  };


}
