#pragma once

#include <gtools/matrices/abstract.hpp>

#include <glm/glm.hpp>

namespace gtools {
  namespace matrices {

    struct view_subjective : public abstract_object<view_subjective>
    {
     public:

     public:
      glm::vec3 position; // La caméra est à ..., dans l'espace monde
      glm::vec2 direction_angle; // horizontal/vertical
      glm::vec3 up;      // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers)

     public:
      inline view_subjective() {reset();}
      inline view_subjective( const glm::vec3 _position, const glm::vec2 _direction_angle, const glm::vec3 _up ) :
        position( std::move( _position ) ), direction_angle( std::move( _direction_angle ) ), up( std::move( _up ) )
      { compute(); }

     public:
      inline const mat4x4_t& compute() override {
        glm::vec3 direction(
          std::cos( direction_angle.y ) * std::sin( direction_angle.x ),
          std::sin( direction_angle.y ),
          std::cos( direction_angle.y ) * std::cos( direction_angle.x )
        );
        m = glm::lookAt( position, direction, up );
        return m;
      }

     protected:
      inline const view_subjective& get_default() const override {
        static const view_subjective default_view {{ 0.f, 0.f, 8.f }, { 0.f, 0.f }, { 0.f, 1.f, 0.f }};
        return default_view;
      }
    };


  }
}

