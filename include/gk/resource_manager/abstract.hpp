#pragma once

#include <erc/package_id.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace gk {
  namespace resource_manager {

    template<typename TResource, typename TKey = std::string>
    struct abstract
    {
      //public:
      // const erc::package_id package_id;
      //public:
      // typedef  TResource( *constructor_pf_t )( const std::string & erc_path, const TConstructArgs & ... args );
      // const constructor_pf_t constructor_pf;



     public:
      //abstract_resource_manager( const erc::package_id _package_id, const constructor_pf_t _constructor_pf ) :
      //  package_id( std::move( _package_id ) ),
      //  constructor_pf( _constructor_pf )
      //{}
      // abstract_resource_manager( const erc::package_id _package_id ) :
      //   package_id( std::move( _package_id ) )
      // {}

     public:
      using index_key = TKey;
      using resource_type = TResource;
      using constructors_function = std::function<resource_type ()>;

     protected:
      struct resource_block
      {
        constructors_function constructors_function;
        std::unique_ptr<resource_type> resource;
        resource_type& get() { if( !resource ) resource = constructors_function(); }
      };

     protected:
      using indexof_resources_t = std::unordered_map<std::string, std::unique_ptr<resource_block>>;
      indexof_resources_t resources;

     public:
      void emplace_constructor( std::string identifier, constructors_function&& cf )
      {
        //
        typename indexof_resources_t::iterator it_find( resources.find( identifier ) );
        if( it_find != resources.end() )
          throw std::runtime_error( "[gk::resource_manager::abstract::emplace_constructor] Can't emplace constructor '" + identifier + "', because it already exists." );
        //
        resources.emplace( std::move( identifier ), { std::move( cf ), nullptr } );
      }

     public:
      template<typename TContructor>
      TResource& base_get( const std::string& identifier )
      {

        //
        typename indexof_resources_t::iterator it_find( resources.find( identifier ) );
        if( it_find == resources.end() )
          throw std::runtime_error( "[gk::resource_manager::abstract::base_get] No identifier '" + identifier + "' constructor found!" );

        //
        return it_find->get();

        //
        // if( cf_it_find == indexof_constructors_function.end() )
        // //
        // typename indexof_resources_t::iterator cf_it_find( indexof_constructors_function.find( identifier ) );
        //
        // std::unique_ptr<TResource> resource_up( constructor_up() );
        // TResource& res_constructed_ref( *resource_up );
        // constructed_resource_by_identifier_ups.emplace( identifier, std::move( resource_up ) )
        // return res_constructed_ref;
      }

    };


    //  protected:
    //   using constructed_resource_by_name_ups_t = std::map<std::string, std::unique_ptr<TResource>>;
    //   constructed_resource_by_name_ups_t constructed_resource_by_identifier_ups;
    //
    //  public:
    //   template<typename TContructor>
    //   TResource& base_get( const std::string& identifier, const TContructor& constructor_up )
    //   {
    //
    //     //
    //     typename constructed_resource_by_name_ups_t::iterator local_build_found( constructed_resource_by_identifier_ups.find( identifier ) );
    //     if( local_build_found != constructed_resource_by_identifier_ups.end() )
    //       return *local_build_found->second;
    //
    //     //
    //     std::unique_ptr<TResource> resource_up( constructor_up() );
    //
    //     TResource& res_constructed_ref( *resource_up );
    //     constructed_resource_by_identifier_ups.emplace( identifier, std::move( resource_up ) );
    //
    //     return res_constructed_ref;
    //   }
    //
    // };

  }
}
