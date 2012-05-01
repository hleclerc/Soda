#ifndef CPPSESSION_H
#define CPPSESSION_H

#include "../Database/Session.h"

/**
*/
class CppSession : public Session {
public:
    CppSession( Database *db, User *user, int num_inst );
};

#endif // CPPSESSION_H
