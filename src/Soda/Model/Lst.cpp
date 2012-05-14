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

void Lst::write_ujs( Stream &out, Session *session ) const {
    // need creation ?
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ]->watching_sessions.has( session ) )
            _data[ i ]->write_njs( out, 0, session );
    // Lst instructions
    out << "var tmp = FileSystem._objects[ " << this << " ];\n";
    for( int i = 0; i < _data.size(); ++i )
        out << "tmp.set_or_push( " << i << ", FileSystem._objects[ " << _data[ i ] << " ] );\n";
    out << "tmp.trim( " << _data.size() << " );\n";
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

bool Lst::_write_njs( Stream &out, int var, Session *session ) const {
    out << "var v_" << var << " = new " << type() << ";\n";
    for( int i = 0; i < _data.size(); ++i ) {
        if ( not _data[ i ]->write_njs( out, var + 1, session ) )
            return false;
        out << "v_" << var << ".push( v_" << var + 1 << " );\n";
    }
    return true;
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
}
