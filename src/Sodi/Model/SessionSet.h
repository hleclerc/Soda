#ifndef SESSIONSET_H
#define SESSIONSET_H

#include "../Sys/IdSet.h"
class Database;
class Session;

/**
*/
class SessionSet : public IdSet<Session *,1,Database *> {
public:
    typedef IdSetList<Session *,1,Database *> List;
};

#endif // SESSIONSET_H
