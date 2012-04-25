#include "../Database/TmpModelMap.h"
#include "../Database/Database.h"
#include "../Database/MapRead.h"
#include "../Sys/BinInp.h"
#include "../Sys/BinOut.h"
#include "ModelWithAttr.h"

ModelWithAttr::ModelWithAttr( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions  ) {
    int nb_attr = inp->read();
    _data.resize( nb_attr );
    for( int i = 0; i < nb_attr; ++i ) {
        _data[ i ].key = inp->read();
        _data[ i ].val = inp->read();
    }
}

ModelWithAttr::ModelWithAttr( RightSet rights, SessionSet watching_sessions ) : Model(  rights, watching_sessions  ) {
}

int ModelWithAttr::nb_attr() const {
    return _data.size();
}

Model *ModelWithAttr::attr( int index ) const {
    return _data[ index ].val;
}

Model *ModelWithAttr::attr( Nstring name ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( _data[ i ].key == name )
            return _data[ i ].val;
    return 0;
}

Model *ModelWithAttr::attr( StringBlk name ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( _data[ i ].key == name )
            return _data[ i ].val;
    return 0;
}

void ModelWithAttr::map_ptr( const MapRead &map_read ) {
    if ( op_id == cur_op_id )
        return;
    op_id = cur_op_id;

    _map_ptr( map_read );
}

void ModelWithAttr::map_ptr( const TmpModelMap &tmp_map, Session *s ) {
    for( int i = 0; i < _data.size(); ++i )
        if ( not ( _data[ i ].val = tmp_map( _data[ i ].val, s ) ) )
            _data.remove( i-- );
}


void ModelWithAttr::write_ujs( Stream &out, Session *session ) const {
    // need some object creation ?
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ].val->watching_sessions.has( session ) )
            _data[ i ].val->write_njs( out, 0, session );
    // Lst instructions
    out << "FileSystem._objects[ " << this << " ].set_attr( {\n";
    for( int i = 0; i < _data.size(); ++i )
        out << "  " << _data[ i ].key << ": FileSystem._objects[ " << _data[ i ].val << " ],\n";
    out << "} );\n";
}

void ModelWithAttr::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
    for( int i = 0; i < _data.size(); ++i ) {
        _data[ i ].key = map_read[ _data[ i ].key ];
        _data[ i ].val = map_read[ _data[ i ].val ];
        if ( _data[ i ].val )
            _data[ i ].val->map_ptr( map_read );
    }
}

void ModelWithAttr::write_str( Stream &out ) const {
    out << '[' << type() << "] ";
    for( int i = 0; i < _data.size(); ++i )
        _data[ i ].val->write_str( out << ( i ? "," : "" ) << _data[ i ].key << ":" );
}

void ModelWithAttr::write_dmp( BinOut &out ) const {
    out << (int)_data.size();
    for( int i = 0; i < _data.size(); ++i ) {
        out << _data[ i ].key;
        out << _data[ i ].val;
    }
}

bool ModelWithAttr::_set( StringBlk data ) {
    Vec<Item> tmp;
    while ( StringBlk p = data.split( ',' ) ) {
        StringBlk k = p.split( ':' );
        if ( p and k ) {
            Item &res = tmp.push_back();
            res.key = db()->nstring_list( k );
            res.val = (Model *)p.atoi();
        }
    }
    if ( _data == tmp )
        return false;
    _data = tmp;
    return true;
}

bool ModelWithAttr::_add_attr( Session *s, StringBlk name, Model *m ) {
    return _add_attr( s, s->db->nstring_list( name ), m );
}

bool ModelWithAttr::_add_attr( Session *s, Nstring name, Model *m ) {
    Item item;
    item.key = name;
    item.val = m;
    _data << item;
    return true;
}

void ModelWithAttr::_write_njs( Stream &out, int var, Session *session ) const {
    out << "var v_" << var << " = new " << type() << ";\n";
    for( int i = 0; i < _data.size(); ++i ) {
        _data[ i ].val->write_njs( out, var + 1, session );
        out << "v_" << var << ".mod_attr( '" << _data[ i ].key << "', v_" << var + 1 << " );\n";
    }
}
