#include "../Database/Session.h"
#include "HttpRequest_Public.h"

#include <Celo/StringHelp.h>
#include <sstream>

HttpRequest_Public::HttpRequest_Public( int fd, ServerLoop *loop ) : EventObj_HttpRequest( fd ), oun( out ), loop( loop ) {
    oun << "HTTP/1.0 200 OK";
    oun << "Content-Type: text/javascript";
    //oun << "Content-Length: 00000000";
    oun << "";
    //end_num = out.str().size() - 3;

    session = 0;
}

int HttpRequest_Public::parse( char *beg, char *end ) {
    return 1;
}

void HttpRequest_Public::hup() {
    PRINT( "hup" );
    TODO;
//    if ( session and session->push_channel == this )
//        session->db->rem_session( session );
}

void HttpRequest_Public::cmd_new_session( int num_inst ) {
    session = (JavascriptSession *)loop->database->session_allocator.factory( loop->database, loop->database->root_usr, num_inst );
    tmp_map.session = session;

    // set _session_num
    oun << "FileSystem._insts[ " << num_inst << " ]._session_num = " << session << ";";

    // in case some data has been buffered before obtaining the session id
    oun << "FileSystem._timeout_send_func();";

    // set up a "push" channel
    oun << "FileSystem._insts[ " << num_inst << " ].make_channel();";
}

void HttpRequest_Public::cmd_set_session( ST ptr_session ) {
    session = dynamic_cast<JavascriptSession *>( loop->database->session_allocator.check( reinterpret_cast<Session *>( ptr_session ) ) );
    tmp_map.session = session;
}

int HttpRequest_Public::cmd_end() {
    // data preparation
    String res = out.str();
    char *data = (char *)res.data();
    int size = res.size();
    //for( int c = size - end_num - 3; c; c /= 10 )
    //    data[ end_num-- ] = '0' + ( c % 10 );
    
    // send
    // std::cout.write( data, size );
    send_str( data, size );
    return OK;
}

//
void HttpRequest_Public::cmd_creation( const String &type, ST tmp_id ) {
    if ( session and tmp_id & 3 ) {
        Model *res = session->factory( session->db->nstring_list( type.c_str() ) );
        out << "FileSystem._tmp_id_to_real( " << tmp_id << ", " << res << " );\n";
        tmp_map.data[ tmp_id ] = res;
    }
}

void HttpRequest_Public::cmd_change( ST ptr_model, const String &data ) {
    if ( session ) {
        MP mp( session, tmp_map[ ptr_model ] );
        mp.set( tmp_map, StringBlk( data.data(), data.size() ) );
    }
}

void HttpRequest_Public::cmd_load( const String &path, int num_callback ) {
    if ( session and session->user )  {
        if ( Model *m = session->operator[]( StringBlk( path.data(), path.size() ) ) ) {
            m->write_njs( out, 0, session );
            oun << "FileSystem._callbacks[ " << num_callback << " ]( v_0, false );";
            return;
        }
    }
    oun << "FileSystem._callbacks[ " << num_callback << " ]( undefined, true );";
}

void HttpRequest_Public::cmd_save( const String &path, ST ptr_model ) {
    if ( session )
        session->operator[]( StringBlk( path.data(), path.size() ) ) = tmp_map[ ptr_model ];
}

void HttpRequest_Public::mk_chan( ST ptr_session ) {
    if ( JavascriptSession *jss = dynamic_cast<JavascriptSession *>( loop->database->session_allocator.check( reinterpret_cast<Session *>( ptr_session ) ) ) ) {
        session = jss;
        if ( not jss->push_channel ) {
            jss->push_channel = this;

            String s = jss->data_to_push.str();
            if ( s.size() ) {
                out << s;
                jss->rq_chan_and_close_pc();
            } else
                loop->add_timeout( 30, jss );
        }
    }
}

void HttpRequest_Public::rq_chan_and_close() {
    if ( JavascriptSession *jss = dynamic_cast<JavascriptSession *>( session ) )
        oun << "FileSystem._insts[ " << jss->num_inst << " ].make_channel();";
    cmd_end();
}
