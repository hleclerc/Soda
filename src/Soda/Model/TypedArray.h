#ifndef TYPEDARRAY_H
#define TYPEDARRAY_H

#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"
#include "../Sys/BinRd.h"
#include "../Sys/S.h"
#include "Model.h"

/**
*/
template<class T>
class TypedArray : public Model {
public:
    TypedArray( RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    }

    TypedArray( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
        _size.resize( (int)inp->read() );
        for( int i = 0; i < dim(); ++i )
            _size[ i ] = inp->read();

        _data.resize( nb_items() );
        for( unsigned i = 0; i < _data.size(); ++i )
            _data[ i ] = inp->read();
    }

    virtual void write_str( Stream &out ) const {
        out << '[';
        for( int i = 0; i < dim(); ++i )
            out << _size[ i ];
        out << ']';
        for( unsigned i = 0; i < _data.size(); ++i )
            out << _data[ i ];
    }

    virtual void write_dmp( BinOut &out ) const {
        out << dim();
        for( int i = 0; i < dim(); ++i )
            out << size( i );

        for( int i = 0, m = nb_items(); i < m; ++i )
            out << _data[ i ];
    }

    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const {
        uut << "FileSystem._objects[" << this << "].set('" << dim();
        for( int i = 0; i < dim(); ++i )
            uut << ',' << size( i );
        uut.precision( 16 );
        for( int i = 0, m = nb_items(); i < m; ++i )
            uut << ',' << _data[ i ];
        uut << "');\n";
        return true;
    }

    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const {
        int ts = sizeof( int ) * ( 1 + _size.size() ) + sizeof( T ) * nb_items();
        uut << 'W' << PI64( this ) << ts;

        uut << (int)_size.size();
        for( unsigned i = 0; i < _size.size(); ++i )
            uut << _size[ i ];

        for( int i = 0; i < nb_items(); ++i )
            uut << _data[ i ];
        return true;
    }

    virtual int type_dump() const {
        return _type_dump( S<T>() );
    }

    virtual Nstring type() const {
        return _type( S<T>() );
    }

    virtual bool _set( const TmpModelMap &mm, StringBlk n ) {
        std::vector<int> os = _size;
        std::vector<T  > od = _data;

        StringBlk d = n.split( ',' );
        _size.resize( d.atoi() );
        for( int i = 0; i < dim(); ++i )
            _size[ i ] = n.split( ',' ).atoi();

        _data.resize( nb_items() );
        for( unsigned i = 0; i < _data.size(); ++i )
            _data[ i ] = n.split( ',' ).read_FP64();

        return os != _size or od != _data;
    }

    virtual bool _set( const char *str, int len ) {
        BinRd rd( str, len );
        int n;
        rd >> n;
        _size.resize( n );
        for( int i = 0; i < n; ++i )
            rd >> _size[ i ];

        _data.resize( nb_items() );
        for( int i = 0; i < nb_items(); ++i )
            rd >> _data[ i ];
        return true;
    }

    virtual void _map_ptr( const MapRead &map_read ) {
        rights = map_read[ rights ];
    }

    virtual void _sweeper_rec( Sweeper &s ) {
    }

    int dim() const {
        return _size.size();
    }

    int size( int d ) const {
        return _size[ d ];
    }

    int nb_items() const {
        if ( not _size.size() )
            return 0;
        int m = _size[ 0 ];
        for( unsigned i = 1; i < _size.size(); ++i )
            m *= _size[ i ];
        return m;
    }

    int _type_dump( S<FP64> ) const { return _TypedArray_Float64; }
    int _type_dump( S<SI32> ) const { return _TypedArray_Int32; }

    Nstring _type( S<FP64> ) const { return NSTRING_TypedArray_Float64; }
    Nstring _type( S<SI32> ) const { return NSTRING_TypedArray_Int32; }


    std::vector<int> _size;
    std::vector<T  > _data;
};

typedef TypedArray<FP64> TypedArray_Float64;
typedef TypedArray<SI32> TypedArray_Int32;


#endif // TYPEDARRAY_H
