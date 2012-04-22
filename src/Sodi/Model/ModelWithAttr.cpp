#include "../Sys/BinOut.h"
#include "ModelWithAttr.h"
#include "Database.h"

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
    TODO;
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
    TODO;
    return false;
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
