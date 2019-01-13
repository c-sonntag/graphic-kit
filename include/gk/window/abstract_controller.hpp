#pragma once

#include <gk/types.hpp>
#include <gk/enum_bits.hpp>

#include <glm/vec3.hpp>

namespace gk {
  namespace window {

    enum class key : uint {
      A=65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    };

    gk_enum_bits( modifier, uchar,
      ControlLeft  = 0b00000010,
      ControlRight = 0b00000001,
      Control      = ControlLeft | ControlRight,
      ShiftLeft    = 0b00001000,
      ShiftRight   = 0b00000100,
      Shift        = ShiftLeft | ShiftRight,
      AltLeft      = 0b00100000,
      AltRight     = 0b00010000,
      Alt          = AltLeft | AltRight,
   /**< @todo   WithGui      = 0b01000000, */
      None         = 0b00000000,
    )

    gk_enum_bits( mouse_button, uint16_t,
      left = 1,
      middle = 2,
      right = 4
    )


  }
}

gk_enum_bits_operator( gk::window::modifier, gk::uchar )
gk_enum_bits_operator( gk::window::mouse_button, uint16_t )

// ---- ---- ---- ----

namespace gk {
  namespace window {

    struct abstract_controller
    {
     public:
      virtual ~abstract_controller() = default;

     public:
      virtual void active_cursor( bool enable = true ) = 0;

      virtual void set_cursor_position( const glm::dvec2& position ) = 0;

      virtual const glm::dvec2& cursor_position() const = 0;

      virtual const glm::dvec2& mouse_wheel() const = 0;

      virtual const glm::uvec2& screen_size() const = 0;

      virtual bool is_clicked( const window::mouse_button& b ) = 0;
      virtual bool is_pressed( const window::key& k ) = 0;

      virtual gk::window::modifier modifier() = 0;

     public:
      virtual bool have( gk::window::modifier m )
      {
        const gk::window::modifier k( modifier() );
        return
          ( m == k ) ?
          true :
          any( k & m )
        ;
      }
    };

  }
}
