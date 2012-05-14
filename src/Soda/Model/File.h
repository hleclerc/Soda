#ifndef FILE_H
#define FILE_H

#include "ModelWithAttr.h"

/**
*/
class File : public ModelWithAttr {
public:
    File( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    File( RightSet rights, SessionSet watching_sessions );
    virtual int type_dump() const { return _File; }
    virtual Nstring type() const;
};

#endif // FILE_H
