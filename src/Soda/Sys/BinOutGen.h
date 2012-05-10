#ifndef BINOUTGEN_H
#define BINOUTGEN_H

/**
*/
template<class OS>
class BinOutGen {
public:
    BinOutGen( OS &os ) : _f( os ) {}

    operator bool() const { return _f; }

    void close() { _f.close(); }
    void flush() { _f.flush(); }

    template<class T>
    BinOutGen &operator<<( const T &res ) {
        _f.write( reinterpret_cast<const char *>( &res ), sizeof res );
        return *this;
    }

    void write( const char *data, int size ) {
        _f.write( data, size );
    }

    OS &_f;
};

#endif // BINOUTGEN_H
