#include <gk/random.hpp>

#include <random>
#include <chrono>

namespace gk {
  namespace random {

    static std::ranlux48_base rand_generator( unsigned(std::chrono::system_clock::now().time_since_epoch().count() ) );
    static std::uniform_real_distribution<float> positive_normal_distribution( 0.01f, 1.f );
    static std::uniform_real_distribution<float> both_distribution( -1.f, 1.f );

    float positive_normal_value( const float coef )
    { return positive_normal_distribution( rand_generator ) * coef; }

    glm::vec3 normal_position( const float coef )
    { return coef * glm::vec3( both_distribution( rand_generator ), both_distribution( rand_generator ), both_distribution( rand_generator ) ); }

  }
}

