#pragma once

#include <gk/matrices/abstract.hpp>

#include <vector>

namespace gk {
  namespace matrices {

    struct composed_matrices : public abstract
    {
     public:
      using matrices_p_t = std::vector<abstract*>;
      const matrices_p_t matrices_p;

     protected:
      inline void unpack( matrices_p_t& ms, abstract& matrice )
      { ms.emplace_back( &matrice ); }

      template<typename ... TArgs>
      inline void unpack( matrices_p_t& ms, abstract& matrice, TArgs& ... matrices )
      { unpack( ms, matrice ); unpack( ms, matrices ... ); }

     protected:
      template<typename ... TArgs>
      matrices_p_t unargs( TArgs& ... matrices ) {
        matrices_p_t matrices_p;
        unpack( matrices_p, matrices ... );
        return matrices_p;
      }

     public:
      template<typename ... TArgs>
      composed_matrices( TArgs& ... matrices ) :
        matrices_p( unargs( matrices ... ) )
      {compute();}

     public:
      virtual ~composed_matrices() = default;

     public:
      inline void compute_only_submatrices() {
        for( abstract* matrice_p : matrices_p )
          matrice_p->compute();
      }

      inline mat4x4_t& compute_only_composition() {
        m = glm::mat4( 1.f );
        for( abstract* matrice_p : matrices_p )
          m *= matrice_p->m;
        return m;
      }

     public:
      inline const mat4x4_t& compute() override {
        compute_only_submatrices();
        compute_only_composition();
        return m;
      }

      inline const mat4x4_t& reset() override {
        m = glm::mat4();
        for( abstract* matrice_p : matrices_p ) {
          matrice_p->reset();
          m *= matrice_p->m;
        }
        return m;
      }

    };

  }
}
