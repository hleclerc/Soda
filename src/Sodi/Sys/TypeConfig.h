#ifndef TYPECONFIG_H
#define TYPECONFIG_H

#include <stdint.h>

typedef float       FP32;
typedef double      FP64;
typedef long double FP80;

typedef bool        Bool;

typedef int8_t      SI8 ;
typedef int16_t     SI16;
typedef int32_t     SI32;
typedef int64_t     SI64;

typedef uint8_t     PI8 ;
typedef uint16_t    PI16;
typedef uint32_t    PI32;
typedef uint64_t    PI64;

template<class P>
struct NI {
    NI( P data ) : data( data ) {}
    P data;
};

typedef NI<PI8 >    NI8 ; ///< negative or null unsigned number
typedef NI<PI16>    NI16; ///< negative or null unsigned number
typedef NI<PI32>    NI32; ///< negative or null unsigned number
typedef NI<PI64>    NI64; ///< negative or null unsigned number


namespace Private {
    template<int N> struct SigSChoice {};
    template<> struct SigSChoice<4> { typedef SI32 T; };
    template<> struct SigSChoice<8> { typedef SI64 T; };

    template<int N> struct PosSChoice {};
    template<> struct PosSChoice<4> { typedef PI32 T; };
    template<> struct PosSChoice<8> { typedef PI64 T; };
}

typedef Private::SigSChoice<sizeof(void *)>::T ST; ///< signed size type
typedef Private::PosSChoice<sizeof(void *)>::T PT; ///< unsigned size type


#endif // TYPECONFIG_H
