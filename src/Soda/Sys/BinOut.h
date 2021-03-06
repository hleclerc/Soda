#ifndef BINOUT_H
#define BINOUT_H

#include <stdlib.h>
#include <string.h>
#include <string>
#include "Math.h"

/**
*/
class BinOut {
public:
    enum { base_rese = 128 };

    BinOut() : _size( 0 ), _rese( 0 ) {}
    ~BinOut() { if ( _rese ) free( _data ); }

    BinOut &operator<<( const std::string &str ) {
        *this << int( str.size() );
        return write( str.data(), str.size() );
    }

    BinOut &operator<<( const BinOut &bou ) {
        return write( bou.data(), bou.size() );
    }

    template<class T>
    BinOut &operator<<( const T &res ) {
        write( reinterpret_cast<const char *>( &res ), sizeof res );
        return *this;
    }

    BinOut &write( const char *ptr, int len ) {
        if ( _rese < _size + len ) {
            if ( _rese ) {
                if ( ( _rese *= 2 ) < _size + len )
                    _rese = _size + len;
                _data = (char *)realloc( _data, _rese );
            } else {
                _rese = len < base_rese ? base_rese : len;
                _data = (char *)malloc( _rese );
            }
        }
        memcpy( _data + _size, ptr, len );
        _size += len;
        return *this;
    }

    void clear( int lim_rese = 1024 ) {
        if ( _rese > lim_rese ) {
            if ( _rese )
                free( _data );
            _rese = 0;
        }
        _size = 0;
    }

    const char *data() const { return _data; }
    int size() const { return _size; }

protected:
    char *_data;
    int   _size;
    int   _rese;
};

#endif // BINOUT_H
