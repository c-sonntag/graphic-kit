#pragma once

#include <types.hpp>

#include <raiigl/gl_types.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
//#include <glm/.hpp>

#include <bits/move.h>
#include <typeinfo>
#include <map>
#include <stdexcept>

namespace vao {

  struct attrib_type
  {
   public:
    const uint tuple_size;
    const raiigl::data_type tuple_type;
    const uint bytesize;

   public:
    template<typename BaseType>
    inline constexpr attrib_type(
      const uint _tuple_size,
      const raiigl::data_type _tuple_type,
      const BaseType &
    ) :
      tuple_size( _tuple_size ),
      tuple_type( _tuple_type ),
      bytesize( sizeof( BaseType ) * tuple_size ) { }
  };

}

// ---- ---- ---- ----

inline bool operator == ( const vao::attrib_type & lhs, const vao::attrib_type & rhs )
{ return ( lhs.tuple_size == rhs.tuple_size ) && ( lhs.tuple_type == rhs.tuple_type ) && ( lhs.bytesize == rhs.bytesize );}

inline bool operator != ( const vao::attrib_type & lhs, const vao::attrib_type & rhs )
{ return !( lhs == rhs ); }

// ---- ---- ---- ----

namespace vao {
  namespace attrib_list {
    static constexpr attrib_type vec3{3, raiigl::data_type::Float, float() };
    static constexpr attrib_type uv2{2, raiigl::data_type::Float, float() };
    static constexpr attrib_type normal3{3, raiigl::data_type::Float, float() };
  }
}

// ---- ---- ---- ----

namespace vao {

  struct location_map : protected std::map<uint, attrib_type>
  {
   public:
    struct assign
    {
      const uint index;
      const attrib_type & t;
      inline assign( const uint _index, const attrib_type & _t ) :
        index( _index ), t( _t ) {}
    };

   protected:
    uint m_total_attrib_bytesize = 0;

   public:
    inline uint total_attrib_bytesize() const { return m_total_attrib_bytesize; }

   protected:
    inline void unpack( const assign & a ) {
      if ( find( a.index ) != end() )
        throw std::runtime_error( "[vao::attrib_conf::requirement] Index '" + std::to_string( a.index ) + "' already used" );
      emplace( a.index, a.t );
      m_total_attrib_bytesize += a.t.bytesize;
    }

    template<typename ... Args>
    inline void unpack( const assign & a, const Args ... args )
    { unpack( a ), unpack( args... ); }

   public:
    template<typename ... Args>
    inline location_map( const Args ... args )
    { unpack( args... ); }

   public:
    const attrib_type & require_index( const uint index ) const {
      location_map::const_iterator req_cit( find( index ) );
      if ( req_cit == end() )
        throw std::runtime_error( "[vao::attrib_conf::requirement] Index '" + std::to_string( index ) + "' not can be found" );
      return req_cit->second;
    }

   public:
    /** @todo optimise that : */
    bool is_compatible_for( const location_map & l ) const {
      for ( const std::pair<uint, attrib_type> & p : *this ) {
        location_map::const_iterator req_cit( l.find( p.first ) );
        if ( req_cit == end() ) return false;
        if ( req_cit->second != p.second ) return false;
      }
      return true;
    }

    // public:
    //  uint get_offset( const uint index ) const {
    //    uint offset = 0;
    //    for ( uint i( 0 ); i < index; ++i ) {
    //      location_map::const_iterator req_cit( find( i ) );
    //      if ( req_cit == end() )
    //        throw std::runtime_error( "[vao::attrib_conf::requirement] Index '" + std::to_string( i ) + "' not can be found to compute offset of Index '" + std::to_string( index ) + "'" );
    //      offset += req_cit->second.bytesize;
    //    }
    //    return offset;
    //  }

  };

}

// struct requirement : protected std::map<uint, type>
// {
//  public:
//   using std::map<uint, type>::map;
//  public:
//
// };
//
// struct compatible : protected std::map<uint, type>
// {
//  public:
//   using std::map<uint, type>::map;
//  public:
//   bool compatible_with( const requirement & r );
// };

// ---- ---- ---- ----

namespace vao {

  namespace pack {

    struct vec3_uv2
    {
      glm::vec3 vec3;
      glm::vec2 uv2;

      vec3_uv2() = default;

      // explicit vec3_uv2( glm::vec3 && _vec3, glm::vec3 && _uv2 ) :
      //   vec3( std::move( _vec3 ) ), uv2( std::move( _uv2 ) ) {}

      //vec3_uv2( const glm::vec3 & _vec3, const glm::vec3 & _uv2 ) :
      //  vec3( _vec3 ), uv2( _uv2 ) {}
    };

    struct vec3_uv2_norm3
    {
      glm::vec3 vec3;
      glm::vec2 uv2;
      glm::vec3 norm3;

      vec3_uv2_norm3() = default;

      //explicit vec3_uv2_norm3( glm::vec3 _vec3, glm::vec2 _uv2, glm::vec3 _norm3 ) :
      //  vec3( std::move( _vec3 ) ), uv2( std::move( _uv2 ) ), norm3( std::move( _norm3 ) ) {}

      explicit vec3_uv2_norm3( glm::vec3 && _vec3, glm::vec2 && _uv2, glm::vec3 && _norm3 ) :
        vec3( std::move( _vec3 ) ), uv2( std::move( _uv2 ) ), norm3( std::move( _norm3 ) ) {}

      vec3_uv2_norm3( const glm::vec3 & _vec3, const glm::vec2 & _uv2, const glm::vec3 & _norm3 ) :
        vec3( _vec3 ), uv2( _uv2 ), norm3( _norm3 ) {}
    };

    // ---- ---- ---- ----

    using uint3 = glm::highp_uvec3;

    // struct uint3
    // {
    //   uint v1;
    //   uint v2;
    //   uint v3;
    //
    //   uint3() = default;
    //
    //   explicit uint3( uint && _v1, uint && _v2, uint && _v3 ) :
    //     v1( std::move( _v1 ) ), v2( std::move( _v2 ) ), v3( std::move( _v3 ) ) {}
    //
    //   explicit uint3( const uint & _v1, const uint & _v2, const uint & _v3 ) :
    //     v1( _v1 ), v2( _v2 ), v3( _v3 ) {}
    // };

  }

}



