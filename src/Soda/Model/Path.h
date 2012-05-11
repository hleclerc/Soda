#ifndef PATH_H
#define PATH_H

#include "ModelWithAttr.h"

/**
*/
class Path : public ModelWithAttr {
public:
    Path( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    Path( RightSet rights, SessionSet watching_sessions );

    virtual void write_dmp( BinOut &out ) const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    String path; ///< file id
};

#endif // PATH_H
