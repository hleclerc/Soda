#ifndef SODASESSION_H
#define SODASESSION_H

#include "Session.h"
class SodaRequest_Public;

/**
*/
class SodaSession : public Session {
public:
    SodaSession( Database *db, User *user, SodaRequest_Public *req );

    virtual void on_reg_type( Model *m, int n_callback );
    virtual void on_change( Model *m );
    virtual void end_round();

    SodaRequest_Public *req;
};

#endif // SODASESSION_H
