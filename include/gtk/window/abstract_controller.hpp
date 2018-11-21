#pragma once

#include <gtk/types.hpp>
#include <gtk/enum_bits.hpp>

#include <glm/vec3.hpp>

namespace gtk {
  namespace window {

    enum class key : uint {
      A=65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    };

    gtk_enum_bits( key_modifier, uchar,
      Control      = 0b00000011,
      ControlLeft  = 0b00000010,
      ControlRight = 0b00000001,
      Shift        = 0b00001100,
      ShiftLeft    = 0b00001000,
      ShiftRight   = 0b00000100,
      Alt          = 0b00110000,
      AltLeft      = 0b00100000,
      AltRight     = 0b00010000,
    )

  }
}

gtk_enum_bits_operator( gtk::window::key_modifier, gtk::uchar )

// ---- ---- ---- ----

namespace gtk {
  namespace window {

    struct abstract_controller
    {
     public:
      virtual ~abstract_controller() = default;

     public:
      virtual void active_cursor( bool enable = true ) = 0;
      virtual void set_cursor( const glm::uvec2& position ) = 0;
      virtual bool key_pressed( const window::key& k ) = 0;
      virtual key_modifier key_modifier() = 0;
    };

  }
}
