#include "HttpRequest_Public.h"
#include "JavascriptSession.h"

JavascriptSession::JavascriptSession( Database *db, User *user, int num_inst ) : Session( db, user ), num_inst( num_inst ) {
    push_channel = 0;
    list = 0;
}

void JavascriptSession::on_reg_type( Model *m ) {
    PRINT( "yop !" );
}

void JavascriptSession::on_change( Model *m ) {
    if ( m->mod_session != this ) {
        Stream &nut = push_channel ? push_channel->out : data_to_push;
        std::ostringstream uut;
        if ( m->write_ujs( nut, uut, this ) )
            nut << uut.str();
    }
}

void JavascriptSession::rq_chan_and_close_pc() {
    if ( list )
        *list >> this;

    if ( push_channel ) {
        push_channel->rq_chan_and_close();
        delete push_channel;
        push_channel = 0;
    }
}

void JavascriptSession::end_round() {
    rq_chan_and_close_pc();
}
