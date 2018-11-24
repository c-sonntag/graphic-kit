#pragma once

namespace gk {
  namespace render {

    class abstract
    {
     public:
      virtual ~abstract() = default;

     public:
      virtual void prepare() { /* nothing */ }
      virtual void execute() = 0;
    };

  }
}


