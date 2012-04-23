#ifndef USER_H
#define USER_H

#include "ModelWithAttr.h"

/**
*/
class User : public ModelWithAttr {
public:
    User( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    User( RightSet rights, SessionSet watching_sessions );
    virtual int type_dump() const { return _User; }
    virtual Nstring type() const;
};

#endif // USER_H
