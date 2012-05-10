#ifndef BINOUT_H
#define BINOUT_H

#include <fstream>

/**
*/
class BinOut {
public:
    struct AppTrunc {};

    BinOut( const char *file, AppTrunc ) : _f( file, std::ios_base::trunc ) {}
    BinOut( const char *file ) : _f( file ) {}
    BinOut() {}

    operator bool() const { return _f; }

    void open( const char *file, AppTrunc ) { _f.open( file, std::ios_base::trunc ); }
    void open( const char *file ) { _f.open( file ); }

    void close() { _f.close(); }
    void flush() { _f.flush(); }

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
