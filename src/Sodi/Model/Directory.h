#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Lst.h"

/**
*/
class Directory : public Lst {
public:
    Directory( RightSet rights, SessionSet watching_sessions );
    virtual Nstring type() const;

    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;

protected:
    virtual bool _add_attr( Session *s, StringBlk name, Model *m );
    virtual bool _add_attr( Session *s, Nstring name, Model *m );
};

#endif // DIRECTORY_H
