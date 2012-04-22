#ifndef FILE_H
#define FILE_H

#include "ModelWithAttr.h"

/**
*/
class File : public ModelWithAttr {
public:
    File( RightSet rights, SessionSet watching_sessions );
    virtual Nstring type() const;
};

#endif // FILE_H
