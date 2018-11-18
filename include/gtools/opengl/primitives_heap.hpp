#pragma once

#include <raiigl/gl_types.hpp>
#include <raiigl/gl330.hpp>
#include <raiigl/gl430.hpp>
#include <raiigl/program.hpp>
#include <raiigl/buffer.hpp>
#include <raiigl/vertex_array.hpp>

#include <gtools/types.hpp>

#include <gtools/opengl/primitives_heap/attrib_pointer.hpp>
#include <gtools/opengl/primitives_heap/attrib_pointer_by_offset.hpp>

#include <gtools/opengl/primitives_heap/vertex_buffer.hpp>
#include <gtools/opengl/primitives_heap/index_buffer.hpp>
#include <gtools/opengl/primitives_heap/uniform_container.hpp>

#include <gtools/opengl/primitives_heap/abstract_expander_property_support.hpp>

#include <vector>
#include <array>
#include <list>


namespace gtools {
  namespace opengl {

    template<typename  ... TListTypes>
    struct vertex_expander;

    template<typename  ... TListTypes>
    struct index_expander;

    struct uniform_lap;

    // ---- ---- ---- ----

    template<typename  ... TListTypes>
    class primitives_heap
    {
     public:
      static constexpr uint nb_types { sizeof...( TListTypes ) };

     public:
      using primitives_heap_t = primitives_heap<TListTypes...>;

     public:
      using vertices_t = opengl::vertex_buffer<TListTypes...>;
      using indices_t = opengl::index_buffer;
      using uniforms_laps_up_t = std::vector<abstract_expander_property_support::uniform_container_up_t>;

     protected:
      vertices_t vertices;
      indices_t indices;

     protected:
      using attrib_pointers_t = attribs_pointers_by_offset<TListTypes...>;
      const attrib_pointers_t attrib_pointers;

     public:
      template< typename... Args >
      primitives_heap( Args ... attributes );

     protected:
      bool initialized;

     public:
      void init_buffer();
      void reset_buffer();
      void reset_expanders();
      void reset_uniforms_laps();
      void reset_all();

     protected:
      bool vao_is_configured;
      raiigl::vertex_array vao;

     protected:
      void gl_attrib_pointer( raiigl::gl330 & gl );
      void attrib_array_enable_all( raiigl::gl330 & gl );
      void attrib_array_disable_all( raiigl::gl330 & gl );

     public: /**< @todo */ // protected
      virtual void bind_buffer();

     public:
      //void configure_vao( raiigl::gl330 & gl, raiigl::program & program );

      void draw( raiigl::gl330 & gl, raiigl::program & program );
      void multiple_instance_draw( raiigl::gl430 & gl, raiigl::program & program, const uint nb_instance );

     public:
      using abstract_expander_property_up_t = std::unique_ptr<opengl::abstract_expander_property>;

     protected:
      bool busy = false;
      std::vector<abstract_expander_property_up_t> expanders_properties;
      uniforms_laps_up_t uniforms_laps;

     protected:
      void check_not_busy() const;

     public:
      bool empty() const;

     protected:
      struct expander_property_support_inherited : public opengl::abstract_expander_property_support
      {
       protected:
        primitives_heap_t & heap;

       private:
        inline void check_busy();

       public:
        expander_property_support_inherited( primitives_heap_t & _heap );
        void lock();
        void unlock( abstract_expander_property_up_t property_up );
        void unlock( uniform_container_up_t uniform_container_up );
      } property_support;

     public:
      bool is_init() const;
      bool have_uniforms_laps() const;
      bool is_busy() const;

     public:
      conditional_uniform_set & conditionals_uniforms_sets;

     public:
      using vertex_expander = opengl::vertex_expander<TListTypes...>;
      using index_expander = opengl::index_expander<TListTypes...>;
      using uniform_lap = opengl::uniform_lap;

     public:
      vertex_expander complete_primitive( raiigl::primitive_type primitive );
      index_expander complete_indexed_primitive( raiigl::primitive_type primitive );

     public:
      uniform_lap complete_uniform_lap();

    };

  }
}


#include <gtools/opengl/primitives_heap.ipp>
