#include "ModelWithAttrAndName.h"
#include "../Sys/BinOut.h"

ModelWithAttrAndName::ModelWithAttrAndName( RightSet rights, SessionSet watching_sessions, Nstring _type ) : ModelWithAttr( rights, watching_sessions ), _type( _type ) {
}

void ModelWithAttrAndName::write_dmp( BinOut &out ) const {
    ModelWithAttr::write_dmp( out );
    out << _type;
}

Nstring ModelWithAttrAndName::type() const {
    return _type;
}
