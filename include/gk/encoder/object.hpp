#pragma once

#include <gk/types.hpp>
#include <gk/classes/non_copyable.hpp>

#include <erc/file.h>
#include <erc/file_id.h>
#include <erc/inventory_package.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <string>
#include <memory>


namespace gk {
  namespace encoder {

    struct object_importer_option
    {
      bool force_texture2d_coords = true;
      bool do_not_finalize_element = false;
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
      static object load_from_memory( const std::string& input_data,  const object_importer_option& option = {} );
      static object load_from_stream( std::istream& is,  const object_importer_option& option = {} );
      static object load_from_erc( const erc::embedded_file& erc,  const object_importer_option& option = {} );

      static __forceinline object load_from_local_erc( const erc::file_id& erc_id, const object_importer_option& option = {} );

     public:
      struct element
      {
       public:
        std::string name;

       public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;

       public:
        bool use_texture3d = false;
        std::vector<glm::vec2> texture2d_coords;
        std::vector<glm::vec3> texture3d_coords;

       public:
        bool have_indices = false;
        std::vector<uint> vertex_indices;
        std::vector<uint> tex_coord_indices;
        std::vector<uint> normal_indices;

       public:
        bool is_finalized = false;
        std::vector<glm::vec3> finalized_vertices;
        std::vector<glm::vec3> finalized_normals;
        std::vector<glm::vec2> finalized_texture2d_coords;
        std::vector<glm::vec3> finalized_texture3d_coords;

       public:
        inline element( object& _p ) : parent( _p ) {}

       public:
        element( element&& ) = default;

       protected:
        friend object;
        object& parent;
        void parse( uint line_number, const std::string& line );
        void finalise_it();

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
      object( std::istream& is,  const object_importer_option _option );
    };

    // ---- ----

    __forceinline object object::load_from_local_erc( const erc::file_id& erc_id,  const object_importer_option& option )
    {
      const erc::embedded_file& erc( erc::inventory_package::get_local_first_embedded_file( erc_id, "gk::encoder::object::load_from_local_erc" ) );
      object o( load_from_erc( erc, option ) );
      erc.unallocate_proper_data();
      return o;
    }

  }
}
