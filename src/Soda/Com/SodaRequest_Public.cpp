#include "SodaRequest_Public.h"
#include "ServerLoop.h"

#include "../Database/Session.h"
#include "../Sys/BinOut.h"
#include "../Sys/Stream.h"

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ), loop( loop ) {
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr );
}

void SodaRequest_Public::cmd_load( int n_callback, char *path_str, int path_len ) {
    BinOut b;
    if ( session and session->user ) {
        if ( Model *m = session->operator[]( StringBlk( path_str, path_len ) ) ) {
            if ( m->write_nsr( b, session ) ) { // <- checks rights
                b << 'L' << n_callback << SI64( m );
                return;
            }
        }
    }
    b << 'L' << n_callback << SI64( 0 );
}

void SodaRequest_Public::cmd_end() {
    send_str( b.data(), b.size() );
}
