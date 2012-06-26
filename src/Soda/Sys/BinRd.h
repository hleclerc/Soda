#ifndef BINRD_H
#define BINRD_H

/**
Simple binary reader
*/
class BinRd {
public:
    BinRd( const char *data, int size ) : _data( data ), _size( size ) {
    }

    template<class T>
    BinRd &operator>>( T &val ) {
        if ( _size >= (int)sizeof( T ) ) {
            val = *reinterpret_cast<const T *>( _data );
            _data += sizeof( T );
            _size -= sizeof( T );
        }
        return *this;
    }

protected:
    const char *_data;
    int _size;
};

#endif // BINRD_H
