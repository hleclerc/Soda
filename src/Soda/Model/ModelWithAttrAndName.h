#ifndef MODELWITHATTRANDNAME_H
#define MODELWITHATTRANDNAME_H

#include "ModelWithAttr.h"

/**
*/
class ModelWithAttrAndName : public ModelWithAttr {
public:
    ModelWithAttrAndName( RightSet rights, SessionSet watching_sessions, Nstring _type );
    ModelWithAttrAndName( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    virtual int type_dump() const { return _ModelWithAttrAndName; }
    virtual void write_dmp( BinOut &out ) const;
    Nstring type() const;

    virtual void _map_ptr( const MapRead &map_read );

    Nstring _type;
};

#endif // MODELWITHATTRANDNAME_H
