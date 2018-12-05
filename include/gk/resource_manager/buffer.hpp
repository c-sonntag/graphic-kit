#pragma once

#include <gk/resource_manager/abstract.hpp>

#include <gk/types.hpp>

#include <raiigl/buffer.hpp>

#include <memory>
#include <list>

namespace gk {
  namespace resource_manager {

    struct buffers
    {
     protected:
      struct assoc_vec
      {
       public:
        std::size_t total_bytesize = 0;
        raiigl::buffer buffer;

       public:
        inline assoc_vec(
          const raiigl::buffer_type _type,
          const raiigl::buffer_usage _usage
        ) :
          buffer( std::move( _type ), std::move( _usage ) )
        {}
      };

     public:
      const uint buffer_limit;

     protected:
      std::list<assoc_vec> m_buffers;

     public:
      inline buffers( uint _buffer_limit = 0 ) :
        buffer_limit( std::move( _buffer_limit ) )
      {}

     protected:
      inline assoc_vec& get_or_create(
        const std::size_t reserve_bytesize,
        const raiigl::buffer_type type = raiigl::buffer_type::Array,
        const raiigl::buffer_usage usage = raiigl::buffer_usage::StaticDraw
      )
      {
        for( assoc_vec& a : m_buffers )
        {
          if( ( type == a.buffer.type ) && ( usage == a.buffer.usage ) )
          {
            if( buffer_limit == 0  )
              return a;
            else if( ( a.total_bytesize + reserve_bytesize ) < buffer_limit )
              return a;
          }
        }
        m_buffers.emplace_back( type, usage );
        return m_buffers.back();
      }

     public:
      inline raiigl::buffer& get(
        const std::size_t reserve_bytesize,
        const raiigl::buffer_type type = raiigl::buffer_type::Array,
        const raiigl::buffer_usage usage = raiigl::buffer_usage::StaticDraw
      )
      {
        assoc_vec& assoc( get_or_create( reserve_bytesize, type, usage ) );
        assoc.total_bytesize += reserve_bytesize;
        return assoc.buffer;
      }

    };

  }
}
