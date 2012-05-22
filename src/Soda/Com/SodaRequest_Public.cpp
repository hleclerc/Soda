#include "SodaRequest_Public.h"
#include "ServerLoop.h"

#include "../Database/Session.h"
#include "../Sys/Stream.h"

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ), loop( loop ) {
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr );
}

void SodaRequest_Public::cmd_reg_type( int n_callback, const char *type_str, int type_len ) {
    PRINT( StringBlk( type_str, type_len ) );
}

void SodaRequest_Public::cmd_load( int n_callback, const char *path_str, int path_len ) {
    if ( session and session->user ) {
        if ( Model *m = session->operator[]( StringBlk( path_str, path_len ) ) ) {
            BinOut c;
            if ( m->write_nsr( b, c, session ) ) { // <- checks rights
                b << c;
                b << 'L' << SI64( m ) << n_callback;
                return;
            }
        }
    }
    b << 'L' << SI64( 0 ) << n_callback;
}

void SodaRequest_Public::cmd_end() {
    // std::cout << "CMD END" << b.size() << std::endl;
    send_str( b.data(), b.size() );
    b.clear();
}
