#pragma once

#include <gk/matrices/composed_matrices.hpp>

namespace gk {
  namespace matrices {

    struct base_camera : public composed_matrices
    {
     public:
      abstract& projection;
      abstract& view;

     public:
      base_camera( abstract& _projection, abstract& _view ) :
        composed_matrices( _projection, _view ),
        projection( _projection ),
        view( _view )
      {}

     public:
      virtual ~base_camera() = default;

    };

  }
}
