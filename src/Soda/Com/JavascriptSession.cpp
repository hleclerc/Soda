#include "HttpRequest_Public.h"
#include "JavascriptSession.h"

JavascriptSession::JavascriptSession( Database *db, User *user, int num_inst ) : Session( db, user ), num_inst( num_inst ) {
    push_channel = 0;
}

void JavascriptSession::on_change( Model *m ) {
    if ( m->mod_session != this )
        m->write_ujs( push_channel ? push_channel->out : data_to_push, this );
}

void JavascriptSession::rq_chan_and_close_pc() {
    if ( push_channel ) {
        push_channel->rq_chan_and_close();
        delete push_channel;
        push_channel = 0;
    }
}

void JavascriptSession::end_round() {
    rq_chan_and_close_pc();
}
