#include "SodaRequest_Public.h"
#include "ServerLoop.h"

#include "../Database/SodaSession.h"
#include "../Sys/Stream.h"

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ), loop( loop ) {
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr, this );
}

SodaRequest_Public::~SodaRequest_Public() {
    loop->database->remove_refs_to( session );
}

void SodaRequest_Public::cmd_reg_type( int n_callback, const char *type_str, int type_len ) {
    if ( session and session->user )
        session->db->reg_type( StringBlk( type_str, type_len ), session, n_callback );
}

void SodaRequest_Public::cmd_load_ptr( int n_callback, PI64 ptr ) {
    if ( session and session->user ) {
        PRINT( ptr );
        if ( Model *m = session->db->model_allocator.check( reinterpret_cast<Model *>( ptr ) ) ) {
            PRINT( "ptr" );
            BinOut c;
            if ( m->write_nsr( b, c, session ) ) { // <- checks rights
                b << c << 'L' << SI64( m ) << n_callback;
                return;
            }
        }
    }
    b << 'L' << SI64( 0 ) << n_callback;
}

void SodaRequest_Public::cmd_load( int n_callback, const char *path_str, int path_len ) {
    if ( session and session->user ) {
        if ( Model *m = session->operator[]( StringBlk( path_str, path_len ) ) ) {
            BinOut c;
            if ( m->write_nsr( b, c, session ) ) { // <- checks rights
                b << c << 'L' << SI64( m ) << n_callback;
                return;
            }
        }
    }
    b << 'L' << SI64( 0 ) << n_callback;
}

void SodaRequest_Public::cmd_end() {
    send();
}

bool SodaRequest_Public::cnt_default_value() const {
    return true;
}

void SodaRequest_Public::send() {
    if ( b.size() ) {
        send_str( b.data(), b.size() );
        b.clear();
    }
}
