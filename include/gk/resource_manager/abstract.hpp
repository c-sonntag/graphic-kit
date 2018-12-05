#pragma once

#include <erc/package_id.h>

#include <memory>
#include <map>

namespace gk {
  namespace resource_manager {

    template<typename TResource>
    struct abstract
    {
      //public:
      // const erc::package_id package_id;
      //public:
      // typedef  TResource( *constructor_pf_t )( const std::string & erc_path, const TConstructArgs & ... args );
      // const constructor_pf_t constructor_pf;

     protected:
      using constructed_resource_by_name_ups_t = std::map<std::string, std::unique_ptr<TResource>>;
      constructed_resource_by_name_ups_t constructed_resource_by_identifier_ups;

     public:
      //abstract_resource_manager( const erc::package_id _package_id, const constructor_pf_t _constructor_pf ) :
      //  package_id( std::move( _package_id ) ),
      //  constructor_pf( _constructor_pf )
      //{}
      // abstract_resource_manager( const erc::package_id _package_id ) :
      //   package_id( std::move( _package_id ) )
      // {}

     public:
      template<typename TContructor>
      TResource& base_get( const std::string& identifier, const TContructor& constructor_up )
      {

        //
        typename constructed_resource_by_name_ups_t::iterator local_build_found( constructed_resource_by_identifier_ups.find( identifier ) );
        if( local_build_found != constructed_resource_by_identifier_ups.end() )
          return *local_build_found->second;

        //
        std::unique_ptr<TResource> resource_up( constructor_up() );

        TResource& res_constructed_ref( *resource_up );
        constructed_resource_by_identifier_ups.emplace( identifier, std::move( resource_up ) );

        return res_constructed_ref;
      }

    };

  }
}
