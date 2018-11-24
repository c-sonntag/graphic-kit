#pragma once

#include <gk/types.hpp>
#include <gk/classes/non_copyable_movable.hpp>
#include <gk/color/rgba.hpp>

#include <glm/vec2.hpp>

#include <cstdio>
#include <bits/move.h>
#include <sstream>
#include <string>
#include <stdexcept>

namespace gk {
  namespace encoder {

    /**
     * @brief The ffmpeg_command_line struct :
     *  To output video stream on FFMPEG soft encoder
     *  by command line
     * @see http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/
     */
    struct ffmpeg_command_line
    {
     public:
      struct argument
      {
       public:
        std::string ffmpeg_path{ "ffmpeg" };
        glm::uvec2 resolution { 1280, 720 };
        uint refresh_rate{ 60 };
        // bool vflip{ true };

       public:
        inline std::string made_cmd( const std::string& output_path ) const
        {
          std::stringstream ss;
          ss << "\"" << ffmpeg_path << "\"";
          ss << " -r 60 -f rawvideo -pix_fmt";
          ss << " rgba -s " << resolution.x << "x" << resolution.y << " -i - ";
          ss << " -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip";
          ss << " \"" << output_path << "\"";
          return ss.str();
        }
      };

     public:
      const argument cmds;
      const std::string command_line;
      const uint buffer_sizeof;

     protected:
      inline static FILE* open_process( const std::string& cmd_line )
      {
        FILE* const process( _popen( cmd_line.c_str(), "w" ) );
        if( !process ) throw std::runtime_error( "[gk::encoder::ffmpeg_command_line] Can't execute command line : " + cmd_line );
        return process;
      }
      inline static void close_process( FILE* process )
      { _pclose( process ); }

     protected:
      FILE* const ffmpeg_process;
      color::rgba* const m_buffer;

     public:
      inline ffmpeg_command_line(
        argument _cmds,
        const std::string& output_path
      ) :
        cmds( std::move( _cmds ) ),
        command_line( cmds.made_cmd( output_path ) ),
        buffer_sizeof( sizeof( color::rgba ) * cmds.resolution.x * cmds.resolution.y ),
        ffmpeg_process( open_process( command_line ) ),
        m_buffer( new color::rgba[cmds.resolution.x * cmds.resolution.y] )
      { }

     public:
      inline ~ffmpeg_command_line()
      {
        delete[] m_buffer;
        if( ffmpeg_process )
          close_process( ffmpeg_process );
      }

     public:
      gtk_classes_non_copyable_movable( ffmpeg_command_line )

     public:
      inline void write()
      { fwrite( m_buffer, buffer_sizeof, 1, ffmpeg_process ); }

     public:
      inline color::rgba* buffer() { return m_buffer; }
      inline const color::rgba* buffer() const { return m_buffer; }
    };


  }
}
