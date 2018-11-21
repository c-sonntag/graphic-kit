#pragma once

#include <gtk/math.hpp>
#include <gtk/time.hpp>

#include <glm/vec3.hpp>

namespace gtk {

  namespace abstract_equation {
    struct by_time { virtual glm::vec3 compute( const gtk::time& t ) = 0; };
  }

  namespace equations {

    struct time_circle : public abstract_equation::by_time
    {
      float rayon;
      inline time_circle( float _rayon ) : rayon( std::move( _rayon ) ) {}
      virtual glm::vec3 compute( const gtk::time& t ) override
      { return rayon * glm::vec3( std::cos( t.seconds_elpased ), -std::sin( t.seconds_elpased ), 0.f ); }
    };

    //  struct time_equation_elipse : public time_equation
    //  {
    //    glm::

    //    time_equation_elipse()
    //    virtual glm::vec3 compute( const gtk::time& t ) = 0;

    //  };

  }

}


