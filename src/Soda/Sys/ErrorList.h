#ifndef ERRORLIST_H
#define ERRORLIST_H

#include "sys/Stream.h"
#include "Vec.h"

/**
*/
class ErrorList {
public:
    ErrorList();
    ErrorList &operator<<( String error );

    Vec<String> errors;
};

#endif // ERRORLIST_H
