#pragma once

#include <gk/matrices/abstract.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gk {
  namespace matrices {

    struct model : public abstract_object<model>
    {
     public:
      glm::vec3 position;
      glm::vec3 rotation;
      glm::vec3 scale;

     public:
      inline model() {reset();}
      inline model( const glm::vec3 _position, const glm::vec3 _rotation, const glm::vec3 _scale ) :
        position( std::move( _position ) ), rotation( std::move( _rotation ) ), scale( std::move( _scale ) )
      { compute(); }

     public:
      inline const mat4x4_t& compute() override { /** @todo check order */
        m = glm::mat4( 1.f );
        m = glm::scale( m, scale );
        m = glm::translate( m, position );
        m = glm::rotate( m, rotation.x, glm::vec3( 1.f, 0.f, 0.f ) ); /**< @todo in one step ? */
        m = glm::rotate( m, rotation.y, glm::vec3( 0.f, 1.f, 0.f ) ); /**< @todo in one step ? */
        m = glm::rotate( m, rotation.z, glm::vec3( 0.f, 0.f, 1.f ) ); /**< @todo in one step ? */
        return m;
      }

     protected:
      inline const model& get_default() const override {
        static const model default_model {{ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }};
        return default_model;
      }
    };

  }
}

