#pragma once

#include <gk/types.hpp>
#include <gk/classes/non_copyable.hpp>

#include <erc/file.h>
#include <erc/file_id.h>
#include <erc/inventory_package.h>

#include <gk/vectors.hpp>

#include <vector>
#include <unordered_set>
#include <memory>


namespace gk {
  namespace decoder {

    struct object_importer_option
    {
     public:
      bool force_texture2d_coords = true;
      bool force_finalize_element = false;
      bool convert_quad_face_to_quad_triangle = true;
      std::unordered_set<std::string> only_import_element_name;
      std::unordered_set<std::string> not_import_element_name;

     public:
      object_importer_option() = default;
    };


    /**
     * @brief An Object file opener, for file likes ".obj"
     * @see http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-7-model-loading/
     * @see http://paulbourke.net/dataformats/obj/
     */
    struct object : public gk::classes::non_copyable
    {
     public:
      static object load_from_file( const std::string& file_path, const object_importer_option& option = {} );
      static object load_from_memory( const char* data, const uint size, const object_importer_option& option = {} );
      static object load_from_stream( std::istream& is, const object_importer_option& option = {} );
      static object load_from_erc( const erc::embedded_file& erc, const object_importer_option& option = {} );

      static __forceinline object load_from_local_erc( const erc::file_id& erc_id, const object_importer_option& option = {} );

     public:
      vec::vertices_vector vertices;
      vec::normals_vector normals;

     public:
      bool use_texture3d = false;
      vec::texture2d_coords_vector texture2d_coords;
      vec::texture3d_coords_vector texture3d_coords;

     public:
      struct element
      {
        struct finalized
        {
         public:
          const element& el;

         public:
          vec::vertices_vector vertices;
          vec::normals_vector normals;
          vec::texture2d_coords_vector texture2d_coords;
          vec::texture3d_coords_vector texture3d_coords;

         protected:
          friend element;
          finalized( const element& el );

        };

       public:
        std::string name;

       public:
        /**< like :
         * 1=v
         * 2=v vt
         * 3=v vt vn
         */
        uint face_vec_size_format = 0;
        bool have_indices = false;
        bool have_quad_face = false;
        vec::index_vector vertex_indices;
        vec::index_vector tex_coord_indices;
        vec::index_vector normal_indices;

       protected:
        bool stop_import_face = false;
        std::unique_ptr<finalized> m_finalized_up;

       public:
        inline element( object& _p ) : parent( _p ) {}

       public:
        element( element&& ) = default;

       protected:
        friend object;
        object& parent;

       protected:
        void parse( uint line_number, const std::string& line );
        void close();

       public:
        const finalized& get_finalized();

       private:
        void apply_name( const std::string _name );
      };

     public:
      std::list<element> elements;

     protected:
      element* current_element = nullptr;

     public:
      const object_importer_option option;

     protected:
      object( std::istream& is, const object_importer_option& _option = {} );
    };

    // ---- ----

    __forceinline object object::load_from_local_erc( const erc::file_id& erc_id, const object_importer_option& option )
    {
      const erc::embedded_file& erc( erc::inventory_package::get_local_first_embedded_file( erc_id, "gk::decoder::object::load_from_local_erc" ) );
      object o( load_from_erc( erc, option ) );
      erc.unallocate_proper_data();
      return o;
    }

  }
}
