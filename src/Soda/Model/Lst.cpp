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

void Lst::map_ptr( const MapRead &map_read ) {
    if ( op_id == cur_op_id )
        return;
    op_id = cur_op_id;

    rights = map_read[ rights ];
    for( int i = 0; i < _data.size(); ++i )
        if ( ( _data[ i ] = map_read[ _data[ i ] ] ) )
            _data[ i ]->map_ptr( map_read );
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

int Lst::nb_attr() const {
    return _data.size();
}

Model *Lst::attr( int index ) const {
    return _data[ index ];
}

void Lst::_write_njs( Stream &out, int var, Session *session ) const {
    out << "var v_" << var << " = new Lst;\n";
    for( int i = 0; i < _data.size(); ++i ) {
        _data[ i ]->write_njs( out, var + 1, session );
        out << "v_" << var << ".push( v_" << var + 1 << " );\n";
    }
}

bool Lst::_set( StringBlk data ) {
    TODO;
    return false;
}

bool Lst::_push( Model *m ) {
    if ( not m )
        return false;
    _data << m;
    m->parents << this;
    return true;
}
