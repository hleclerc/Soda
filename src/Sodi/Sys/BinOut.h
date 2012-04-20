#ifndef BINOUT_H
#define BINOUT_H

#include <fstream>

/**
*/
class BinOut {
public:
    struct App {};

    BinOut( const char *file, App ) : _f( file, std::ios_base::app ) {}
    BinOut( const char *file ) : _f( file ) {}
    BinOut() {}

    operator bool() const { return _f; }

    void open( const char *file, App ) { _f.open( file, std::ios_base::app ); }
    void close() { _f.close(); }

    template<class T>
    BinOut &operator<<( const T &res ) {
        _f.write( reinterpret_cast<const char *>( &res ), sizeof res );
        return *this;
    }

    void write( const char *data, int size ) {
        _f.write( data, size );
    }

protected:
    std::ofstream _f;
};

#endif // BINOUT_H
