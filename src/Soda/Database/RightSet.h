#ifndef RIGHTSET_H
#define RIGHTSET_H

#include "../Sys/IdSet.h"
class User;

enum {
    RD = 0, // right to read
    WR = 1  // right to write
};

///
typedef IdSet<User *,2> RightSet;

#endif // RIGHTSET_H
