#include "../Com/SodaRequest_Public.h"
#include "../Model/Model.h"
#include "../Sys/Stream.h"
#include "SodaSession.h"

SodaSession::SodaSession( Database *db, User *user, SodaRequest_Public *req ) : Session( db, user ), req( req ) {
}

void SodaSession::on_reg_type( Model *m, int n_callback ) {
    req->b << 'R' << PI64( m ) << n_callback;
}

void SodaSession::on_change( Model *m ) {
    if ( m->mod_session != this ) {
        BinOut uut;
        if ( m->write_usr( req->b, uut, this ) )
            req->b << uut;
    }
}

void SodaSession::end_round() {
    req->send();
}
