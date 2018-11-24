#pragma once

#include <gk/types.hpp>
#include <gk/render/abstract.hpp>

#include <bits/move.h>
#include <array>

namespace gk {
  namespace render {

    template<uint VDimension, typename TData>
    class data_matrix : public abstract
    {
     public:
      static constexpr uint nb_dimension = VDimension;
      static constexpr uint data_type_sizeof = sizeof( TData );
      using dimensions_t = std::array<uint, nb_dimension>;

     protected:
      inline static uint compute_area( const dimensions_t& dimensions )
      { uint area( 1 ); for( uint d : dimensions ) area *= d; return area; }

     public:
      const dimensions_t dimensions;
      const uint area;
      const uint data_sizeof;

     public:
      TData* const data;

     public:
      inline data_matrix( const dimensions_t _dimensions ) :
        dimensions( std::move( _dimensions ) ),
        area( compute_area( dimensions ) ),
        data_sizeof( area * data_type_sizeof  ),
        data( new TData[area] )
      {}

      ~data_matrix()
      { delete [] data; }

    };

  }
}


