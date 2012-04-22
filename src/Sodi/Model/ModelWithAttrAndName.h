#ifndef MODELWITHATTRANDNAME_H
#define MODELWITHATTRANDNAME_H

#include "ModelWithAttr.h"

/**
*/
class ModelWithAttrAndName : public ModelWithAttr {
public:
    ModelWithAttrAndName( RightSet rights, SessionSet watching_sessions, Nstring _type );
    virtual void write_dmp( BinOut &out ) const;
    Nstring type() const;

    Nstring _type;
};

#endif // MODELWITHATTRANDNAME_H
