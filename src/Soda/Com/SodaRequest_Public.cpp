#include "SodaRequest_Public.h"
#include "ServerLoop.h"

#include "../Database/SodaSession.h"
#include "../Sys/Stream.h"

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ), loop( loop ) {
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr, this );
    tmp_map.session = session;
}

SodaRequest_Public::~SodaRequest_Public() {
    session->inactive = true;
    loop->database->remove_refs_to( session );
}

void SodaRequest_Public::cmd_update_PI32( PI64 model_id, SI32 info ) {
    if ( session and session->user )
        if ( Model *m = tmp_map[ model_id ] )
            if ( m->rights.has( session->user, WR ) and m->_set( info, model_stack, string_stack ) )
                session->db->add_to_mod_list( m, session );
}

void SodaRequest_Public::cmd_update_PI64( PI64 model_id, PI64 info ) {
    if ( session and session->user )
        if ( Model *m = tmp_map[ model_id ] )
            if ( m->rights.has( session->user, WR ) and m->_set( info ) )
                session->db->add_to_mod_list( m, session );
}

void SodaRequest_Public::cmd_update_PI8 ( PI64 model_id, PI8 info ) {
    if ( session and session->user ) {
        if ( Model *m = tmp_map[ model_id ] ) {
            if ( m->rights.has( session->user, WR ) and m->_set( (PI64)info ) )
                session->db->add_to_mod_list( m, session );
        }
    }
}

void SodaRequest_Public::cmd_update_6432( PI64 model_id, SI64 man, SI32 exp ) {
    if ( session and session->user )
        if ( Model *m = tmp_map[ model_id ] )
            if ( m->rights.has( session->user, WR ) and m->_set( man, exp ) )
                session->db->add_to_mod_list( m, session );
}

void SodaRequest_Public::cmd_update_cstr( PI64 model_id, const char *type_str, int type_len ) {
    if ( session and session->user )
        if ( Model *m = tmp_map[ model_id ] )
            if ( m->rights.has( session->user, WR ) and m->_set( type_str, type_len ) )
                session->db->add_to_mod_list( m, session );
}

void SodaRequest_Public::cmd_push_string( const char *str, int len ) {
    string_stack << String( str, str + len );
}

void SodaRequest_Public::cmd_push_model ( PI64 model_id ) {
    if ( session and session->user ) {
        if ( Model *m = tmp_map[ model_id ] ) {
            if ( m->rights.has( session->user, RD ) ) {
                model_stack << m;
                return;
            }
        }
    }
    model_stack << (Model *)0;
}

void SodaRequest_Public::cmd_creation( PI64 tmp_id, const char *type_str, int type_len, const char *type_stu, int type_leu ) {
    if ( session and tmp_id & 3 ) {
        Nstring t = session->db->nstring_list( type_str, type_len );
        Nstring u = session->db->nstring_list( type_stu, type_leu );
        Model *res = session->factory( u, t );
        b << 'T' << tmp_id << PI64( res );
        tmp_map.data[ tmp_id ] = res;
    }
}

void SodaRequest_Public::cmd_creation( PI64 tmp_id, const char *type_str, int type_len ) {
    if ( session and tmp_id & 3 ) {
        Nstring t = session->db->nstring_list( type_str, type_len );
        Model *res = session->factory( t, t );
        b << 'T' << tmp_id << PI64( res );
        tmp_map.data[ tmp_id ] = res;
    }
}

void SodaRequest_Public::cmd_reg_type( int n_callback, const char *type_str, int type_len ) {
    if ( session and session->user )
        session->db->reg_type( StringBlk( type_str, type_len ), session, n_callback );
}

void SodaRequest_Public::cmd_load_ptr( int n_callback, PI64 model_id ) {
    if ( session and session->user ) {
        if ( Model *m = tmp_map[ model_id ] ) {
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
