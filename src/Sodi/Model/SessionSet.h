#ifndef SESSIONSET_H
#define SESSIONSET_H

#include "../Sys/IdSet.h"
class Database;
class Session;

/**
*/
typedef IdSet<Session *,1,Database *> SessionSet;

#endif // SESSIONSET_H
