#pragma once

#include <streambuf>
#include <istream>
#include <string>

namespace gk {

  /**
   * @see https://stackoverflow.com/a/13059195
   */
  struct membuf : std::streambuf
  {
    inline membuf( char const* base, size_t size )
    {
      char* p( const_cast<char*>( base ) );
      this->setg( p, p, p + size );
    }
  };

  /**
   * @see https://stackoverflow.com/a/13059195
   */
  struct imemstream : virtual membuf, std::istream
  {
    inline imemstream( char const* base, size_t size ) :
      membuf( base, size ),
      std::istream( static_cast<std::streambuf*>( this ) ) {}

    inline imemstream( const std::string& s ) :
      membuf( s.data(), s.size() ),
      std::istream( static_cast<std::streambuf*>( this ) ) {}
  };

}
