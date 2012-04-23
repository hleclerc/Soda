#include "ModelWithAttrAndName.h"
#include "../Database/MapRead.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"

ModelWithAttrAndName::ModelWithAttrAndName( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( inp, rights, watching_sessions ) {
    _type = inp->read();
}

ModelWithAttrAndName::ModelWithAttrAndName( RightSet rights, SessionSet watching_sessions, Nstring _type ) : ModelWithAttr( rights, watching_sessions ), _type( _type ) {
}

void ModelWithAttrAndName::write_dmp( BinOut &out ) const {
    ModelWithAttr::write_dmp( out );
    out << _type;
}

Nstring ModelWithAttrAndName::type() const {
    return _type;
}

void ModelWithAttrAndName::map_ptr( const MapRead &map_read ) {
    if ( op_id == cur_op_id )
        return;
    op_id = cur_op_id;

    ModelWithAttr::_map_ptr( map_read );
    _type = map_read[ _type ];
}
