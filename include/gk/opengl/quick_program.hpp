#pragma once

#include <gk/classes/non_copyable_movable.hpp>

#include <raiigl/program.hpp>
#include <raiigl/shader.hpp>

#include <erc/file.h>
#include <erc/file_id.h>

#include <string>
#include <list>
#include <memory>

namespace gk {
  namespace opengl {

    struct quick_program
    {
     public:
      enum class open_mode {
        local_erc_file,
      };

     public:
      static raiigl::shader_type detect_type( const std::string& extension );

     public:
      template<typename TAssoc>
      struct shader_file_assoc
      {

       public:
        const raiigl::shader_type type;
        const TAssoc& assoc;

       public:
        __forceinline shader_file_assoc( raiigl::shader_type _type, const TAssoc& _assoc ) :
          type( std::move( _type ) ), assoc( _assoc ) {}

        __forceinline shader_file_assoc( raiigl::shader_type&& _type, const TAssoc& _assoc ) :
          type( std::move( _type ) ), assoc( _assoc ) {}

       public:
        gtk_classes_non_copyable_movable( shader_file_assoc )
      };

      // public:
      //  static shader_file_assoc<std::string> shader_from_file_path( const std::string& file_path );
      //  static shader_file_assoc<erc::file_id> shader_from_local_erc( const erc::file_id& file_id );

     public:
      struct program_composition
      {
       public:
        struct shader_comp : public raiigl::shader
        {
         public:
          std::string composition_name;

         public:
          template<typename ... Args>
          shader_comp( Args ... args ) :
            raiigl::shader( std::move( args ... ) )
          { composition_name = raiigl::name( type ) + "()"; }

         public:
          shader_comp&& set_composition_name( std::string s ) { composition_name = std::move( s ); return std::move( *this ); }
        };

       public:
        std::list<shader_comp> shaders;

       protected:
        std::vector<const erc::embedded_file*> openned_embedded_files_p;
        const std::list<std::string> shaders_creation_errors;

       protected:
        virtual void check_errors();
        virtual void close();

       protected:
        template<typename TAssoc>
        shader_comp open_from_create_shader( shader_file_assoc<TAssoc> shader_assoc );
        shader_comp open_from_create_shader( shader_file_assoc<erc::embedded_file> shader_assoc );
        shader_comp open_from_create_shader( shader_file_assoc<erc::file_id> shader_assoc );

       public:
        template<typename TAssoc>
        __forceinline raiigl::program open_from( shader_file_assoc<TAssoc> shader_assoc );

        template<typename TAssoc, typename ... Args>
        __forceinline raiigl::program open_from( shader_file_assoc<TAssoc> shader_assoc, Args ... args );

       public:
        __forceinline program_composition()
        {}

        virtual __forceinline ~program_composition()
        {}

      };



     public:
      static __forceinline raiigl::program open_from_sources( const std::string& vertex_src, const std::string& fragment_src );
      static __forceinline raiigl::program open_from_file_path( const std::string& vertex_path, const std::string& fragment_path );

      static __forceinline raiigl::program open_from_erc( const erc::embedded_file& vertex_erc, const erc::embedded_file& fragment_erc );

      static __forceinline raiigl::program open_from_local_erc( const erc::file_id& vertex_erc_id, const erc::file_id& fragment_erc_id )
      { return open_from( shader_file_assoc<raiigl::shader_type::Vertex, erc::file_id>( vertex_erc_id ) ); }

     public:
      static __forceinline raiigl::program open_from_local_erc( std::vector<shader_file_assoc>& shaders ... );

     public:

     public:
      static raiigl::shader_type detect_type( const std::string& extension );

      //  public:
      //   static __forceinline raiigl::program open_from_local_erc( const std::vector<erc::file_id>& file_ids );

      // protected:
      //  template<typename ... Args> static __forceinline raiigl::program open_from_local_erc_deploy( std::list<shader_file_assoc>& shaders, shader_file_assoc shader, Args ... args );
      //  static __forceinline raiigl::program open_from_local_erc_deploy( std::list<shader_file_assoc>& shaders, shader_file_assoc shader );
      //
      // protected:
      //  template<typename ... Args> static __forceinline raiigl::program open_from_local_erc_deploy( erc::file_id file_id, Args ... args );
      //  template<typename ... Args> static __forceinline raiigl::program open_from_local_erc_deploy( std::list<erc::file_id>& file_ids, erc::file_id file_id, Args ... args );
      //  static __forceinline raiigl::program open_from_local_erc_deploy( std::list<erc::file_id>& file_ids, erc::file_id file_id );

      //public:
      // template<typename ... Args>
      // static __forceinline raiigl::program open_from_local_erc( Args ... args );
    };

  }
}

#include <gk/opengl/quick_program.ipp>

