#pragma once

#include <gk/types.hpp>
#include <gk/time.hpp>

#include <chrono>
#include <thread>

namespace gk {

  struct frame_limiter
  {
   public:
    using internal_clock = std::chrono::system_clock;

   private:
    uint fps_limit;
    int64_t microseconds_limit;

   private:
    internal_clock::time_point start;
    internal_clock::time_point last;
    internal_clock::time_point current;

   private:
    int64_t deltatime_milliseconds = 0;
    int64_t deltatime_microseconds = 0;

   public:
    inline frame_limiter( const uint _fps_limit = 33 ) :
      fps_limit( std::move( _fps_limit ) ),
      start( internal_clock::now() ),
      last( start )
    { compute_microseconds_limit(); }

    inline void set_limit( const uint _fps_limit )
    { fps_limit = _fps_limit; compute_microseconds_limit();}

   private:
    inline void compute_microseconds_limit()
    { microseconds_limit = static_cast<int64_t>( 1. / static_cast<double>( fps_limit ) * 1'000'000. ); }

   public:
    inline void cadence()
    {
      //
      current = internal_clock::now();
      deltatime_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>( current - last ).count();
      deltatime_microseconds = std::chrono::duration_cast<std::chrono::microseconds>( current - last ).count();
      last = current;

      //
      if( deltatime_microseconds < microseconds_limit )
        std::this_thread::sleep_for( std::chrono::microseconds( microseconds_limit - deltatime_microseconds ) );
    }

   public:
    inline int64_t current_deltatime_milliseconds() const { return deltatime_milliseconds; }
    inline int64_t current_deltatime_microseconds() const { return deltatime_microseconds; }
    inline float current_deltatime_seconds() const { return float(deltatime_milliseconds) / 1000.f; }

    /** @brief return elpased seconds from frame_limiter build time */
    inline float current_elapsed_seconds() const { return float(std::chrono::duration_cast<std::chrono::milliseconds>( current - start ).count() ) / 1000.f; }


   public:
    gk::time time() const
    { return {  current_elapsed_seconds(), current_deltatime_seconds() }; }

   public:
    inline uint current_fps() const { return static_cast<uint>( 1. / ( static_cast<double>( deltatime_microseconds ) * 0.000'001 ) ); }
  };

}
