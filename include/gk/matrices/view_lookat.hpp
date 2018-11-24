#pragma once

#include <gk/matrices/abstract.hpp>

#include <glm/glm.hpp>


namespace gk {
  namespace matrices {

    struct view_lookat : public abstract_object<view_lookat>
    {
     public:
      glm::vec3 position; // La caméra est à ..., dans l'espace monde
      glm::vec3 direction; // et regarde l'origine
      glm::vec3 angle;   // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers)

     public:
      inline view_lookat() {reset();}
      inline view_lookat( const glm::vec3 _position, const glm::vec3 _direction, const glm::vec3 _angle ) :
        position( std::move( _position ) ), direction( std::move( _direction ) ), angle( std::move( _angle ) )
      { compute(); }

     public:
      inline const mat4x4_t& compute() override {
        m = glm::lookAt( position, direction, angle );
        return m;
      }

     protected:
      inline const view_lookat& get_default() const override {
        static const view_lookat default_view {{ 0.f, 0.f, 8.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }};
        return default_view;
      }
    };


  }
}

