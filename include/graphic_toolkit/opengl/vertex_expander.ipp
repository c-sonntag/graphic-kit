#pragma once

#include <graphic_toolkit/opengl/vertex_expander.h>

#include <graphic_toolkit/opengl/primitives_heap.h>
#include <graphic_toolkit/opengl/index_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::property::property( primitive_type _primitive, size_t _start ) :
      primitive( std::move( _primitive ) ),
      start( td::move( _start ) )
    { }

    template<typename  ... TListTypes>
    inline size_t vertex_expander<TListTypes...>::property::get_count() const
    {
      return m_count;
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::vertex_expander( primitives_heap_t & _primitives_heap, primitive_type _primitive ) :
      primitives_heap( _primitives_heap ),
      vertices( primitives_heap.vertices ),
      property_up( std::make_unique<property>( primitive, vertex_buffer.rows.size() ) )
    {
      vertices.lock();
    }

    template<typename  ... TListTypes>
    inline vertex_expander<TListTypes...>::~vertex_expander()
    {
      property_up->m_count = vertices.rows.size() - start;
      primitives_heap.ex
      vertices.unlock();
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    template< class... Args >
    inline void vertex_expander<TListTypes...>::push( Args && ... args )
    {
      vertices.rows.push( args... );
    }

    template<typename  ... TListTypes>
    inline void vertex_expander<TListTypes...>::reserve( size_t n )
    {
      vertices.rows.reserve( start + n );
    }


    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    template< class... Args >
    inline void vertex_expander<TListTypes...>::set_uniform( const std::string & var_name, Args... values )
    {
      //check();
      //return property_up->insert_uniform_set( var_name, values... );
    }

    template<typename  ... TListTypes>
    template< class... Args >
    inline void vertex_expander<TListTypes...>::set_uniform_on_condition( const std::string & condition_name, const std::string & var_name, Args... values )
    {
      //check();
      //return property_up->insert_conditional_uniform_set( bd, condition_name, initial_cond, var_name, values... );
    }

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    inline index_expander & vertex_expander<TListTypes...>::expand_to_index()
    {
      //
      if ( index_expander_up )
        throw std::runtime_error( "This vertex_expander already contains one index_expander_up" );

      //
      index_expander_up = std::make_unique<index_expander>( *this );

      //
      return *index_expander_up;
    }


  }
}

