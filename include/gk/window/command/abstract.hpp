#pragma once

#include <gk/window/abstract_controller.hpp>
#include <gk/time.hpp>

#include <glm/vec3.hpp>

namespace gk {

  namespace render {
    namespace painter {
      struct abstract;
    }
  }

  namespace window {

    namespace command {

      struct abstract
      {
       protected:
        abstract_controller& controller;

       protected:
        gk::window::modifier m_modifier_required;

       public:
        inline const gk::window::modifier& modifier_required() const { return m_modifier_required; }

       public:
        abstract( abstract_controller& _controller, gk::window::modifier _modifier_required = gk::window::modifier::None ) :
          controller( _controller ),
          m_modifier_required( std::move( _modifier_required ) )
        {}

        virtual ~abstract() = default;

       public:
        virtual void apply( render::painter::abstract& _painter, const gk::time& t ) = 0;

        // public:
        //  virtual void on_mouse_move( const glm::uvec2& pos ) = 0;
      };

    }
  }
}
