#include "HttpRequest_Public.h"
#include "JavascriptSession.h"

JavascriptSession::JavascriptSession( Database *db, User *user, int num_inst ) : Session( db, user ), num_inst( num_inst ) {
    push_channel = 0;
}

void JavascriptSession::on_change( Model *m ) {
}

void JavascriptSession::rq_chan() {
    if ( push_channel ) {
        push_channel->rq_chan();
        delete push_channel;
        push_channel = 0;
    }
}
