#include "../Database/TmpModelMap.h"
#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinInp.h"
#include "../Sys/BinOut.h"
#include "../Sys/Stream.h"
#include "Lst.h"

Lst::Lst( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions  ) {
    int nb_items = inp->read();
    _data.resize( nb_items );
    for( int i = 0; i < nb_items; ++i )
        _data[ i ] = inp->read();
}

Lst::Lst( RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions  ) {
}

void Lst::write_str( Stream &out ) const {
    for( int i = 0; i < _data.size(); ++i ) {
        _data[ i ]->write_str( out );
        if ( i + 1 < _data.size() )
            out << ' ';
    }
}

void Lst::write_dmp( BinOut &out ) const {
    out << (int)_data.size();
    for( int i = 0; i < _data.size(); ++i )
        out << _data[ i ];
}

bool Lst::write_ujs( Stream &nut, Stream &uut, Session *session ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ]->write_njs( nut, uut, session ) )
            return false;
    uut << "var tmp = FileSystem._objects[ " << this << " ];\n";
    for( int i = 0; i < _data.size(); ++i )
        uut << "tmp.set_or_push( " << i << ", FileSystem._objects[ " << _data[ i ] << " ] );\n";
    uut << "tmp.trim( " << _data.size() << " );\n";
    return true;
}

bool Lst::write_usr( BinOut &nut, BinOut &uut, Session *session ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ]->write_nsr( nut, uut, session ) )
            return false;
    for( int i = 0; i < _data.size(); ++i )
        uut << 'P' << PI64( _data[ i ] );
    uut << 'U' << PI64( this ) << PI32( _data.size() );
    return true;
}

Nstring Lst::type() const {
    return NSTRING_Lst;
}

int Lst::nb_attr() const {
    return _data.size();
}

Model *Lst::attr( int index ) const {
    return _data[ index ];
}

bool Lst::_set( const TmpModelMap &mm, StringBlk n ) {
    Vec<Model *> tmp;
    while ( StringBlk p = n.split( ',' ) )
        if ( Model *m = mm[ p.atoi() ] )
            tmp << m;
    if ( _data == tmp )
        return false;
    _data = tmp;
    return true;
}


void Lst::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
    for( int i = 0; i < _data.size(); ++i )
        if ( ( _data[ i ] = map_read[ _data[ i ] ] ) )
            _data[ i ]->map_ptr( map_read );
        else
            _data.remove( i-- );
}
