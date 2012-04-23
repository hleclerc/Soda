#ifndef JAVASCRIPTSESSION_H
#define JAVASCRIPTSESSION_H

#include "../Database/Session.h"
class HttpRequest_Public;

/**
*/
class JavascriptSession : public Session {
public:
    JavascriptSession( Database *db, User *user, int num_inst );
    virtual void on_change( Model *m );
    void rq_chan();

    // for push update
    JavascriptSession *next;
    PI64 date_channel;

    //
    HttpRequest_Public *push_channel;
    std::ostringstream data_to_push;
    int num_inst; ///< num session instance in JS
};

#endif // JAVASCRIPTSESSION_H
