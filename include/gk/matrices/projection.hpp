#pragma once

#include <gk/matrices/abstract.hpp>

#include <glm/glm.hpp>

namespace gk {
  namespace matrices {

    struct projection : public abstract_object<projection>
    {
     public:
      float aspect;
      float fov;    /**< @todo : fov for perspective only ?  */
      float zoom;   /**< @todo : zoom for ortho only ?       */
      bool ortho_mode;

     public:
      float znear, zfar;

     public:
      inline projection() {reset();}
      inline projection( const float _aspect, const float _fov, const float _zoom, const bool _ortho_mode, const float _znear, const float _zfar ) :
        aspect( std::move( _aspect ) ), fov( std::move( _fov ) ), zoom( std::move( _zoom ) ),
        ortho_mode( std::move( _ortho_mode ) ),
        znear( std::move( _znear ) ), zfar( std::move( _zfar ) )
      { compute(); }

     public:
      inline const mat4x4_t& compute() override {
        m = (
          ortho_mode ?
          glm::ortho(
            -zoom * aspect, zoom * aspect,
            -zoom, zoom,
            znear, zfar
          ) :
          glm::perspective(
            fov,
            aspect,
            znear, zfar
          )
        );
        return m;
      }

     protected:
      inline const projection& get_default() const override {
        static const projection default_projection { 4.f / 3.f, 45.f, 5.f, false, 0.001f, 1000.f };
        return default_projection;
      }
    };

  }
}

