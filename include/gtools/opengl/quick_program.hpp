#pragma once

#include <gtools/classes/non_copyable_movable.hpp>

#include <raiigl/program.hpp>

#include <erc/file.h>
#include <erc/file_id.h>

#include <string>
#include <memory>

namespace gtools {
  namespace opengl {

    struct quick_program
    {
     public:
      static inline raiigl::program open_from_sources( const std::string & vertex_src, const std::string & fragment_src );
      static inline raiigl::program open_from_file_path( const std::string & vertex_path, const std::string & fragment_path );
      static inline raiigl::program open_from_erc( const erc::embedded_file & vertex_erc, const erc::embedded_file & fragment_erc );

      static __forceinline raiigl::program open_from_local_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id );
    };

  }
}

#include <gtools/opengl/quick_program.ipp>

