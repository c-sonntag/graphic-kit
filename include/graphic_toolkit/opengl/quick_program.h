#pragma once

#include <graphic_toolkit/classes/non_copyable_movable.hpp>

#include <raiigl/program.hpp>

#include <erc/file.h>
#include <erc/file_id.h>

#include <string>
#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    struct quick_program
    {
     public:
      static inline std::unique_ptr<raiigl::program> open_from_sources( const std::string & vertex_src, const std::string & fragment_src );
      static inline std::unique_ptr<raiigl::program> open_from_file_path( const std::string & vertex_path, const std::string & fragment_path );
      static inline std::unique_ptr<raiigl::program> open_from_erc( const erc::embedded_file & vertex_erc, const erc::embedded_file & fragment_erc );

      static __forceinline std::unique_ptr<raiigl::program> open_from_local_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id );
    };

  }
}

#include <graphic_toolkit/opengl/quick_program.ipp>

