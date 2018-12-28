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

        __forceinline shader_file_assoc( raiigl::shader_type& _type, const TAssoc& _assoc ) :
          type( _type ), assoc( _assoc ) {}

       public:
        gk_classes_non_copyable_movable( shader_file_assoc )
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
          __forceinline shader_comp( Args&& ... args ) : raiigl::shader( std::forward<Args>( args ) ...  ) {}
          __forceinline shader_comp( raiigl::shader&& _s ) : raiigl::shader( std::move( _s ) ) {}

         public:
          gk_classes_non_copyable_movable( shader_comp )

         public:
          shader_comp&& set_composition_name( std::string s ) { composition_name = std::move( s ); return std::move( *this ); }
        };

       public:
        std::list<shader_comp> shaders;

       protected:
        std::vector<const erc::embedded_file*> openned_embedded_files_p;
        std::list<std::string> shaders_creation_errors;

       protected:
        virtual void check_errors();

       protected:
        template<typename TAssoc>
        shader_comp open_from_create_shader( shader_file_assoc<TAssoc> shader_assoc );
        shader_comp open_from_create_shader( shader_file_assoc<erc::embedded_file> shader_assoc );
        shader_comp open_from_create_shader( shader_file_assoc<erc::file_id> shader_assoc );

       protected:
        template<typename TAssoc>
        void open_from( shader_file_assoc<TAssoc> shader_assoc );

        template<typename TAssoc, typename ... Args>
        void open_from( shader_file_assoc<TAssoc> shader_assoc, Args&& ... args );

       public:
        template<typename ... Args>
        program_composition( Args&& ... args );
        virtual ~program_composition();

       public:
        raiigl::program made_program() const;

      };

     public:
      using sfa_src = shader_file_assoc<std::string>;
      using sfa_path = shader_file_assoc<std::string>;
      using sfa_erc = shader_file_assoc<erc::embedded_file>;
      using sfa_erc_id = shader_file_assoc<erc::file_id>;

     public:
      static __forceinline raiigl::program open_from_sources( const std::string& vertex_src, const std::string& fragment_src )
      { return program_composition( sfa_src( raiigl::shader_type::Vertex, vertex_src ), sfa_src( raiigl::shader_type::Fragment, fragment_src ) ).made_program(); }

      static __forceinline raiigl::program open_from_sources( const std::string& vertex_src, const std::string& fragment_src, const std::string& geometry_src )
      { return program_composition( sfa_src( raiigl::shader_type::Vertex, vertex_src ), sfa_src( raiigl::shader_type::Fragment, fragment_src ), sfa_src( raiigl::shader_type::Fragment, geometry_src ) ).made_program(); }

     public:
      static __forceinline raiigl::program open_from_file_path( const std::string& vertex_path, const std::string& fragment_path )
      { return program_composition( sfa_path( raiigl::shader_type::Vertex, vertex_path ), sfa_path( raiigl::shader_type::Fragment, fragment_path ) ).made_program(); }

      static __forceinline raiigl::program open_from_file_path( const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path )
      { return program_composition( sfa_path( raiigl::shader_type::Vertex, vertex_path ), sfa_path( raiigl::shader_type::Fragment, fragment_path ), sfa_path( raiigl::shader_type::Geometry, geometry_path ) ).made_program(); }

     public:
      static __forceinline raiigl::program open_from_erc( const erc::embedded_file& vertex_erc, const erc::embedded_file& fragment_erc )
      { return program_composition( sfa_erc( raiigl::shader_type::Vertex, vertex_erc ), sfa_erc( raiigl::shader_type::Fragment, fragment_erc ) ).made_program(); }

      static __forceinline raiigl::program open_from_erc( const erc::embedded_file& vertex_erc, const erc::embedded_file& fragment_erc, const erc::embedded_file& geometry_erc )
      { return program_composition( sfa_erc( raiigl::shader_type::Vertex, vertex_erc ), sfa_erc( raiigl::shader_type::Fragment, fragment_erc ), sfa_erc( raiigl::shader_type::Geometry, geometry_erc ) ).made_program(); }

     public:
      static __forceinline raiigl::program open_from_local_erc( const erc::file_id& vertex_erc_id, const erc::file_id& fragment_erc_id )
      { return program_composition( sfa_erc_id( raiigl::shader_type::Vertex, vertex_erc_id ), sfa_erc_id( raiigl::shader_type::Fragment, fragment_erc_id ) ).made_program(); }

      static __forceinline raiigl::program open_from_local_erc( const erc::file_id& vertex_erc_id, const erc::file_id& fragment_erc_id, const erc::file_id& geometry_erc_id )
      { return program_composition( sfa_erc_id( raiigl::shader_type::Vertex, vertex_erc_id ), sfa_erc_id( raiigl::shader_type::Fragment, fragment_erc_id ), sfa_erc_id( raiigl::shader_type::Geometry, geometry_erc_id ) ).made_program(); }


      // static __forceinline raiigl::program open_from_local_erc( std::vector<shader_file_assoc>& shaders ... );
     public:

    };

  }
}

#include <gk/opengl/quick_program.ipp>

