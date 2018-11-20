#pragma once

#include <gtk/classes/non_copyable_movable.hpp>

#include <raiigl/shader.hpp>
#include <raiigl/buffer.hpp>

#include <erc/file.h>
#include <erc/file_id.h>

#include <string>
#include <memory>

namespace gtk {
  namespace opengl {

    struct quick_shader
    {
     public:
      static inline raiigl::shader open_from_file_path( const std::string & shader_path, const raiigl::shader_type type );
      static inline raiigl::shader open_from_erc( const erc::embedded_file & shader_erc, const raiigl::shader_type type );

      static __forceinline raiigl::shader open_from_local_erc( const erc::file_id & shader_erc_id, const raiigl::shader_type type );
    };

  }
}

#include <gtk/opengl/quick_shader.ipp>

