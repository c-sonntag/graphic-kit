#pragma once

#include <gk/opengl/primitives_heap/attrib_pointer.hpp>

#include <typeinfo>
#include <vector>

namespace gk {
  namespace opengl {

    struct attrib_pointer_by_offset : public attrib_pointer
    {
     public:
      const std::type_info& ctype;
      GLsizei bytesize;
      size_t offset;
      size_t opposed_offset;

     public:
      template<typename T>
      inline attrib_pointer_by_offset( attrib_pointer va, size_t _offset, size_t row_size, T* );
    };


    template<typename ... TListTypes>
    class attribs_pointers_by_offset : public std::vector<attrib_pointer_by_offset>
    {
     protected:
      template<typename TAttribPointerTypeLast>
      void insert_attributes( size_t offset, attrib_pointer& va );

      template<typename TAttribPointerTypeCurrent, typename ... TAttribPointerTypeNext, typename ... Args>
      void insert_attributes( size_t offset, attrib_pointer& va, Args& ... va_next );

     public:
      template<typename ... Args>
      attribs_pointers_by_offset( Args ... attributes );
    };

  }
}

#include <gk/opengl/primitives_heap/attrib_pointer_by_offset.ipp>
