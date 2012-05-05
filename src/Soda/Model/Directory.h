#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Lst.h"

/**
*/
class Directory : public Lst {
public:
    Directory( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    Directory( RightSet rights, SessionSet watching_sessions );
    virtual int type_dump() const { return _Directory; }
    virtual Nstring type() const;

    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;
};

#endif // DIRECTORY_H
