#ifndef RIGHTSET_H
#define RIGHTSET_H

#include "../Sys/IdSet.h"
class User;

/**
*/
class RightSet : public IdSet<User *,2> {
public:
    typedef IdSetList<User *,2> List;

    RightSet &operator=( const IdSet<User *,2>::Item *p ) { _ptr = p; return *this; }

    enum {
        RD = 0, // right to read
        WR = 1  // right to write
    };

};

#endif // RIGHTSET_H
