#ifndef STRINGBLK_H
#define STRINGBLK_H

#include "TypeConfig.h"
#include <string.h>
#include <string>

/**
  Pointer on string data.

  StringBlk has no destructor. Data is assumed to be owned elsewhere
*/
class StringBlk {
public:
    StringBlk() : _size( 0 ) {}
    StringBlk( const char *data ) : _data( data ), _size( strlen( data ) ) {}
    StringBlk( const char *data, int size ) : _data( data ), _size( size ) {}

    char operator[]( int i ) const { return _data[ i ]; }

    const char *c_str() const { return _data; }
    operator std::string() const { return std::string( _data, _data + _size ); }

    int size() const { return _size; }

    void operator++() { ++_data; --_size; }
    operator bool() const { return _size; }

    bool operator==( const std::string &str ) const {
        return str == std::string( _data, _data + _size );
    }

    StringBlk split( char s ); ///< "toto/tata".split( '/' ) will return toto, and this will be = to tata

    ST read_ST(); // atoi with modified _data and _size
    FP64 read_FP64(); // modified _data and _size

    ST atoi() const;

    template<class OS>
    void write_str( OS &out ) const {
        out.write( _data, _size );
    }

protected:
    const char *_data;
    int _size;
};


#endif // STRINGBLK_H
