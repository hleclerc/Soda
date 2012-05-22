#ifndef BININP_H
#define BININP_H

#include <fstream>
#include <string>
#include "Math.h"

#include "../Sys/Stream.h"
#include <stdlib.h>

/**
*/
class BinInp {
public:
    struct L {
        template<class T> operator T() {
            T res;
            *b >> res;
            return res;
        }
        BinInp *b;
    };

    BinInp( const char *file ) : _f( file ), ok( true ) {}
    operator bool() const { return _f and ok; }
    void close() { _f.close(); }

    ///<
    BinInp &operator>>( std::string &res ) {
        int size = read();
        char *_ata = new char[ size ];
        read( _ata, size );
        res.assign( _ata, _ata + size );
        delete [] _ata;
        return *this;
    }

    ///<
    template<class T>
    BinInp &operator>>( T &res ) {
        read( reinterpret_cast<char *>( &res ), sizeof( T ) );
        return *this;
    }

    ///< Ex: MyType a = bininp.read();
    L read() {
        L l;
        l.b = this;
        return l;
    }

    void read( char *data, int size ) {
        _f.read( data, size );
    }

protected:
    std::ifstream _f;
    bool ok;
};

#endif // BININP_H
