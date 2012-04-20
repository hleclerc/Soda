#ifndef STREAM_H
#define STREAM_H

#include "TypeConfig.h"
#include <iostream>
#include <string>

template<class O,class R> struct ShouldBeDisplayedWriteToStream { typedef R T; };

template<class R> struct ShouldBeDisplayedWriteToStream<Bool,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<char,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<SI8 ,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI16,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<SI64,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<PI8 ,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI16,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<PI64,R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<FP32,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<FP64,R> {};
template<class R> struct ShouldBeDisplayedWriteToStream<FP80,R> {};

template<class O,class R> struct ShouldBeDisplayedWriteToStream<O *,R> {};
template<ST n,class R> struct ShouldBeDisplayedWriteToStream<char [n],R> {};

template<class R> struct ShouldBeDisplayedWriteToStream<std::string,R> {};


template<class T>
typename ShouldBeDisplayedWriteToStream<T,std::ostream>::T &operator<<( std::ostream &os, const T &val ) {
    val.write_str( os );
    return os;
}

#ifndef PRINT
    #define PRINT( A ) \
        std::cout << "  " << #A << " -> " << (A) << std::endl
    #define PRINTN( A ) \
        std::cout << "  " << #A << " ->\n" << (A) << std::endl
    #define TODO \
        std::cerr << "TODO:" << __FILE__ << ":" << __LINE__ << std::endl
#endif

typedef std::string String;
typedef std::ostream Stream;

#endif // STREAM_H
