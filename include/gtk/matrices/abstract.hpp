#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace gtk {
  namespace matrices {

    struct abstract
    {
     public:
      virtual ~abstract() = default;

     public:
      using mat4x4_t = glm::mat4;
      mat4x4_t m;

     public:
      /** @brief glm::Mat4x4 Cast Operator */
      inline operator const glm::mat4& ( ) const { return m; }
      inline operator const void*() const { return &m[0][0]; }

      template<typename T>
      const T* cast() const { return reinterpret_cast<T*>( &m[0][0] ); }

     public:
      virtual const mat4x4_t& compute() = 0;
      virtual const mat4x4_t& reset() = 0;
    };

    // ---- ---- ---- ----

    template<typename TMatrice>
    struct abstract_object : public abstract
    {
     public:
      abstract_object() = default;
      abstract_object( const abstract_object& ) = default;
      abstract_object( abstract_object&& ) = default;
      abstract_object& operator =( const abstract_object& ) = default;
      abstract_object& operator =( abstract_object&& ) = default;
      virtual ~abstract_object() = default;

     protected:
      std::shared_ptr<TMatrice> default_m_up;

     protected:
      virtual const TMatrice& get_default() const = 0;

     private:
      virtual void internal_overwrite( const TMatrice& from )
      { reinterpret_cast<TMatrice&>( *this ) = from; }

     public:
      inline const mat4x4_t& reset() override
      {
        if( default_m_up )
        {
          auto k( default_m_up );
          internal_overwrite( *default_m_up );
          default_m_up = k;
        }
        else
          internal_overwrite( get_default() );
        return m;
      }

     public:
      inline void set_default( const TMatrice& d )
      {
        default_m_up = std::make_unique<TMatrice>( d );
        default_m_up->compute();
      }

    };

  }
}

// ---- ---- ---- ----

inline glm::mat4 operator *( const gtk::matrices::abstract& lhs, const glm::mat4& rhs )               { return lhs.m * rhs; }
inline glm::mat4 operator *( const glm::mat4& lhs, const gtk::matrices::abstract& rhs )               { return lhs * rhs.m; }
inline glm::mat4 operator *( const gtk::matrices::abstract& lhs, const gtk::matrices::abstract& rhs ) { return lhs.m * rhs.m; }
inline glm::mat4 operator /( const gtk::matrices::abstract& lhs, const glm::mat4& rhs )               { return lhs.m / rhs; }
inline glm::mat4 operator /( const glm::mat4& lhs, const gtk::matrices::abstract& rhs )               { return lhs / rhs.m; }
inline glm::mat4 operator /( const gtk::matrices::abstract& lhs, const gtk::matrices::abstract& rhs ) { return lhs.m / rhs.m; }
inline glm::mat4 operator +( const gtk::matrices::abstract& lhs, const glm::mat4& rhs )               { return lhs.m + rhs; }
inline glm::mat4 operator +( const glm::mat4& lhs, const gtk::matrices::abstract& rhs )               { return lhs + rhs.m; }
inline glm::mat4 operator +( const gtk::matrices::abstract& lhs, const gtk::matrices::abstract& rhs ) { return lhs.m + rhs.m; }
inline glm::mat4 operator -( const gtk::matrices::abstract& lhs, const glm::mat4& rhs )               { return lhs.m - rhs; }
inline glm::mat4 operator -( const glm::mat4& lhs, const gtk::matrices::abstract& rhs )               { return lhs - rhs.m; }
inline glm::mat4 operator -( const gtk::matrices::abstract& lhs, const gtk::matrices::abstract& rhs ) { return lhs.m - rhs.m; }

