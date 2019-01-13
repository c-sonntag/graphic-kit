#pragma once

#include <ostream>

#define gk_bit_of( num ) ( 1 << num )

#define gk_enum_bits_value_all( enum_type ) ( ( ( sizeof( enum_type ) ) * 256 ) - 1 )

#define gk_enum_bits_operator( enum_name, enum_type )                                                                  \
  inline enum_name operator |( enum_name a, enum_name b ) {return enum_name( enum_type( a ) | enum_type( b ) );}        \
  inline enum_name operator &( enum_name a, enum_name b ) {return enum_name( enum_type( a ) & enum_type( b ) );}        \
  inline enum_name operator -( enum_name a, enum_name b ) {return enum_name( enum_type( a ) & ( ~enum_type( b ) ) );}   \
  inline enum_name& operator |=( enum_name& a, enum_name b ) {return a = a | b;}                                        \
  inline enum_name& operator &=( enum_name& a, enum_name b ) {return a = a & b;}                                        \
  inline enum_name& operator -=( enum_name& a, enum_name b ) {return a = a - b;}                                        \
  inline bool operator ==( enum_name a, enum_name b ) { return enum_type( a ) == enum_type( b ); }                      \
  inline bool operator !=( enum_name a, enum_name b ) { return enum_type( a ) != enum_type( b ); }                      \
  inline bool operator !( enum_name a ) {return enum_type( a ) == 0;}                                                   \
  inline enum_name operator *( enum_name a, bool cond ) {return cond ? a : enum_name( 0 );}                             \
  inline enum_name operator *( bool cond, enum_name a ) {return cond ? a : enum_name( 0 );}                             \
  inline bool any( enum_name a ) { return enum_type( a ) > 0; }                                                         \
  inline std::ostream& operator <<( std::ostream& os, enum_name a ) { os << int(a); return os; }


#define gk_enum_bits( enum_name, enum_type, ... )                         \
  enum class enum_name : enum_type                                         \
  { _all = gk_enum_bits_value_all( enum_type ), _none = 0, __VA_ARGS__  };



// //PRINT_ENUM_BITS( print_flag )
// std::cout <<  "EnumList(print_flag): " << array_to_string( print_flag_values ) << std::endl;
// std::cout <<  "EnumList(print_flag): " << enum_bits_to_string<print_flag>( print_flag_values ) << std::endl;
//
// std::cout << "  " << std::endl
//           << "  -> action   : " << print_flag::action << std::endl
//           << "  -> original : " << print_flag::original << std::endl
//           << "  -> seuil    : " << print_flag::seuil << std::endl
//           << "  -> seuil    : " << print_flag:: << std::endl
//           << std::endl;


// #include "preprocessor.h"
//
// #include <sstream>
// #include <iomanip>
// #include <bitset>
// template<size_t S>
// static inline std::string array_to_string( const char * const( &values )[S] )
// {
//   std::string values_str;
//   values_str.reserve();
//   for ( const char * const a : values )
//     values_str += "  " + std::string( a );
//   return values_str;
// }
// template<typename TEnum, size_t S>
// static inline std::string enum_bits_to_string( const char * const( &values )[S] )
// {
//   std::stringstream values_ss;
//   unsigned int b( 0 );
//   for ( const char * const a : values )
//   {
//     values_ss << "  " << a << " = 0b" << std::bitset<sizeof( TEnum ) * 8>( b ) << " = " << b;
//     b = b << 1;
//     //b <<= 1;
//   }
//   return values_ss.str();
// }
// #define PRINT_ENUM_BITS(enum_name)
//   std::cout <<  "EnumList( " << #enum_name << "): " << array_to_string( enum_name ) << std::endl;
// // ---- ----
// #define ADD_ENUM_BITS_UNPACK_VARGS(enum_type,bits_count,element,...)
//   element = (1 << bits_count), IF_ELSE( HAS_ARGS( __VA_ARGS__ ) )( DEFER2( ADD_ENUM_BITS_UNPACK_VARGS_s )()( enum_type, (bits_count + 1), __VA_ARGS__ ) )( /* nothing */  )
// #define ADD_ENUM_BITS_UNPACK_VARGS_s() ADD_ENUM_BITS_UNPACK_VARGS
// //static_assert( bits_count & ADD_ENUM_BITS_VALUE_ALL( enum_type ), "[ADD_ENUM_BITS] Actual bits_count(" ## bits_count ## ") not compatible for enum_type(" ## enum_type## ")" )
// //
// // ---- ----
//#define ADD_ENUM_BITS_UNPACK_VALUES_VARGS(x) #x,
// ---- ----
//{ EVAL(ADD_ENUM_BITS_UNPACK_VARGS(enum_type,0,__VA_ARGS__)) _all = ADD_ENUM_BITS_VALUE_ALL(enum_type) };
//static constexpr const char * const enum_name ## _values[]{ EVAL(MAP(ADD_ENUM_BITS_UNPACK_VALUES_VARGS,__VA_ARGS__)) };

// ---- ---- ---- ----
//#define ADD_ENUM_BITS_UNPACK_VARGS_ONCE(element,bits_count)
//  element = ( bits_count << 1 )
//#define ADD_ENUM_BITS_UNPACK_VARGS_ONCE(element,bits_count,...)
//  IF_ELSE( )
//  element = ( bits_count << 1 ), ADD_ENUM_BITS_UNPACK_VARGS_ONCE( __VA_ARGS__ )
// ---- ----
// #define FIRST(element,bits_count, ...) (element = (bits_count << 1))
// #define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
// //#define _END_OF_ARGUMENTS_() 0
//
//ADD_ENUM_BITS( change_flag, uchar, none, mouse, zoom, seuil, action, print, plot )
//ADD_ENUM_BITS( print_flag, uchar, original, seuil, action )
