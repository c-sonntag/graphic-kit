#pragma once

#include <gk/classes/non_copyable_movable.hpp>

#include <raiigl/program.hpp>
#include <raiigl/shader.hpp>

#include <erc/file.h>
#include <erc/file_id.h>

#include <string>
#include <memory>

namespace gk {
  namespace opengl {

    struct composed_program
    {
     public:
      std::vector<raiigl::shader> dependances;

     public:
      template<class ... TOptionalShader>
      raiigl::program new_program(TOptionalShader & ... shaders);

     public:
      inline void clear_dependances() { dependances.clear(); }

     //public:
     // static inline raiigl::program open_from_sources( const std::string & vertex_src, const std::string & fragment_src );
     // static inline raiigl::program open_from_file_path( const std::string & vertex_path, const std::string & fragment_path );
     // static inline raiigl::program open_from_erc( const erc::embedded_file & vertex_erc, const erc::embedded_file & fragment_erc );
     //
     // static __forceinline raiigl::program open_from_local_erc( const erc::file_id & vertex_erc_id, const erc::file_id & fragment_erc_id );
    };

  }
}

#include <gk/opengl/composed_program.ipp>

