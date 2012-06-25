#include "StringBlk.h"
#include "math.h"

StringBlk StringBlk::split( char s ) { ///< "toto/tata".split( '/' ) will return toto, and this will be = to tata
    for( int i = 0; i < _size; ++i ) {
        if ( _data[ i ] == s ) {
            StringBlk res( _data, i );
            _data += i + 1;
            _size -= i + 1;
            return res;
        }
    }
    StringBlk res = *this;
    _size = 0;
    return res;
}

ST StringBlk::read_ST() {
    const char *end = _data + _size;
    ST res = 0;
    if ( *_data == '-' )
        for( ++_data; _data < end and *_data >= '0' and *_data <= '9'; --_size, ++_data )
            res = 10 * res - ( *_data - '0' );
    else
        for( ; _data < end and *_data >= '0' and *_data <= '9'; --_size, ++_data )
            res = 10 * res + ( *_data - '0' );
    return res;
}

ST StringBlk::atoi() const {
    ST res = 0;
    if ( _data[ 0 ] == '-' )
        for( int i = 1; i < _size and _data[ i ] >= '0' and _data[ i ] <= '9'; ++i )
            res = 10 * res - ( _data[ i ] - '0' );
    else
        for( int i = 0; i < _size and _data[ i ] >= '0' and _data[ i ] <= '9'; ++i )
            res = 10 * res + ( _data[ i ] - '0' );
    return res;
}

FP64 StringBlk::read_FP64() { // modified _data and _size
    // -
    if ( _data[ 0 ] == '-' ) {
        --_size; ++_data;
        return - read_FP64();
    }

    double man = read_ST();
    if ( *_data == '.' ) {
        int os = --_size; ++_data;
        double vir = read_ST();
        man += vir / pow( 10.0, os - _size );
    }

    if ( *_data == 'e' )
        man *= pow( 10.0, read_ST() );

    return man;
}
