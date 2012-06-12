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

bool ModelWithAttr::write_ujs( Stream &nut, Stream &uut, Session *session ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ].val->write_njs( nut, uut, session ) )
            return false;
    uut << "FileSystem._objects[ " << this << " ].set_attr( {\n";
    for( int i = 0; i < _data.size(); ++i )
        uut << "  " << _data[ i ].key << ": FileSystem._objects[ " << _data[ i ].val << " ],\n";
    uut << "} );\n";
    return true;
}

bool ModelWithAttr::write_usr( BinOut &nut, BinOut &uut, Session *session ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( not _data[ i ].val->write_nsr( nut, uut, session ) )
            return false;
    for( int i = 0; i < _data.size(); ++i ) {
        uut << 'P' << PI64( _data[ i ].val );
        uut << 'p' << String( _data[ i ].key );
    }
    uut << 'U' << PI64( this ) << PI32( _data.size() );
    return true;
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

bool ModelWithAttr::_set( int size, Vec<Model *> &model_stack, Vec<String> &string_stack ) {
    if ( model_stack.size() < size or string_stack.size() < size )
        return false;

    int os = string_stack.size() - size;
    int om = model_stack.size() - size;

    Vec<Item> tmp;
    for( int i = 0; i < size; ++i ) {
        Item &res = tmp.push_back();
        res.key = db()->nstring_list( string_stack[ os + i ].data(), string_stack[ os + i ].size() );
        res.val = model_stack[ om + i ];
        if ( not res.val ) {
            model_stack.resize( om );
            string_stack.resize( os );
            return false;
        }
    }

    model_stack.resize( om );
    string_stack.resize( os );

    if ( _data == tmp )
        return false;
    _data = tmp;
    return true;
}


bool ModelWithAttr::_set( const TmpModelMap &mm, StringBlk data ) {
    Vec<Item> tmp;
    while ( StringBlk p = data.split( ',' ) ) {
        StringBlk k = p.split( ':' );
        if ( p and k ) {
            if ( Model *m = mm[ p.atoi() ] ) {
                Item &res = tmp.push_back();
                res.key = db()->nstring_list( k );
                res.val = m;
            }
        }
    }
    if ( _data == tmp )
        return false;
    _data = tmp;
    return true;
}

void ModelWithAttr::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
    for( int i = 0; i < _data.size(); ++i ) {
        _data[ i ].key = map_read[ _data[ i ].key ];
        _data[ i ].val = map_read[ _data[ i ].val ];
        if ( _data[ i ].val )
            _data[ i ].val->map_ptr( map_read );
        else
            _data.remove( i-- );
    }
}

void ModelWithAttr::_sweeper_rec( Sweeper &s ) {
    if ( op_id != cur_op_id ) {
        op_id = cur_op_id;
        s( this );

        for( int i = 0; i < _data.size(); ++i )
            if ( Model *f = _data[ i ].val )
                f->_sweeper_rec( s );
    }
}

