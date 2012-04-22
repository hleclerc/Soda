#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/Stream.h"
#include "Lst.h"

Lst::Lst( RightSet rights, SessionSet watching_sessions ) : Model(  rights, watching_sessions  ) {
}

void Lst::map_ptr( const MapRead &map_read ) {
    TODO;
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

Nstring Lst::type() const {
    return NSTRING_Lst;
}

bool Lst::_set( StringBlk data ) {
    TODO;
}

bool Lst::_push( Model *m ) {
    if ( not m )
        return false;
    _data << m;
    m->parents << this;
    return true;
}
