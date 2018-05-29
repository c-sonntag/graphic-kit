#pragma once
#ifndef graphic_toolkit_opengl_abstract_expander_property_support_h
#define graphic_toolkit_opengl_abstract_expander_property_support_h

#include <graphic_toolkit/opengl/uniform_set.h>

#include <list>
#include <memory>

namespace graphic_toolkit {
  namespace opengl {

    struct abstract_expander_property;
    struct uniform_container;

    // ---- ----

    struct abstract_expander_property_support
    {
     public:
      using abstract_expander_property_up_t = std::unique_ptr<abstract_expander_property>;
      using uniform_container_up_t = std::unique_ptr<uniform_container>;

     public:
      virtual ~abstract_expander_property_support() = default;

     public:
      virtual void lock() = 0;
      virtual void unlock( abstract_expander_property_up_t property_up ) = 0;
      virtual void unlock( uniform_container_up_t uniform_container_up ) = 0;

     public:
      conditional_uniform_set conditionals_uniforms_sets;
    };

  }
}

#endif
