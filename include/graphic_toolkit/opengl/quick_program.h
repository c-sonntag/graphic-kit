#pragma once
#ifndef graphic_toolkit_opengl_quick_program_h
#define graphic_toolkit_opengl_quick_program_h

#include <graphic_toolkit/classes/non_copyable_movable.hpp>

#include <raiigl/program.hpp>

#include <erc/file_id.h>

#include <string>

namespace graphic_toolkit {
  namespace opengl {

    struct quick_program
    {
     public:
      static inline raiigl::program open_from_sources( const std::string & vertex_src, const std::string & fragment_src );
      static inline raiigl::program open_from_file_path( const std::string & vertex_path, const std::string & fragment_path );
      static inline raiigl::program open_from_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id );
    };

  }
}

#include <graphic_toolkit/opengl/quick_program.ipp>
#endif

