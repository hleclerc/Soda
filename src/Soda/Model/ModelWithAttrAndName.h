#ifndef MODELWITHATTRANDNAME_H
#define MODELWITHATTRANDNAME_H

#include "ModelWithAttr.h"

/**
*/
class ModelWithAttrAndName : public ModelWithAttr {
public:
    ModelWithAttrAndName( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    ModelWithAttrAndName( RightSet rights, SessionSet watching_sessions, Nstring _type );
    virtual int type_dump() const { return _ModelWithAttrAndName; }
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_dmp( BinOut &out ) const;
    Nstring type() const;

    Nstring _type;
};

#endif // MODELWITHATTRANDNAME_H
