#include "../Database/Session.h"
#include "HttpRequest_Public.h"

#include <Celo/StringHelp.h>
#include <sstream>

HttpRequest_Public::HttpRequest_Public( int fd, SodaLoop *loop ) : EventObj_HttpRequest( fd ), oun( out ), loop( loop ) {
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
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr, num_inst );

    // set _session_num
    oun << "FileSystem._insts[ " << num_inst << " ]._session_num = " << session << ";";

    // in case some data has been buffered before obtaining the session id
    oun << "FileSystem._timeout_send_func();";

    // set up a "push" channel
    oun << "FileSystem._insts[ " << num_inst << " ].make_channel();";
}

void HttpRequest_Public::cmd_set_session( ST ptr_session ) {
    session = dynamic_cast<JavascriptSession *>( loop->database->session_allocator.check( reinterpret_cast<Session *>( ptr_session ) ) );
}

int HttpRequest_Public::cmd_end() {
    for( TmpModelMap::TM::iterator iter = tmp_map.tmp_map.begin(); iter != tmp_map.tmp_map.begin(); ++iter )
        iter->second->map_ptr( tmp_map, session );

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

void HttpRequest_Public::cmd_creation( const String &type, ST tmp_id, const String &data ) {
    if ( session and tmp_id & 3 ) {
        Model *res = session->factory( session->db->nstring_list( type.c_str() ), data );
        out << "FileSystem._tmp_id_to_real( " << tmp_id << ", " << res << " );\n";
        tmp_map.tmp_map[ tmp_id ] = res;
    }
}

void HttpRequest_Public::cmd_change( ST ptr_model, const String &data ) {
    if ( session ) {
        if ( Model *m = tmp_map( ptr_model, session ) ) {
            MP mp( session, m );
            mp = StringBlk( data.data(), data.size() );
        }
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
    if ( not session ) {
        if ( Model *m = tmp_map( ptr_model, session ) ) {
            if ( m->rights.has( session->user, RD ) ) {
                MP mp = session->operator[]( StringBlk( path.data(), path.size() ) );
                mp = m;
            }
        }
    }
}

void HttpRequest_Public::mk_chan( ST ptr_session ) {
    session = dynamic_cast<JavascriptSession *>( loop->database->session_allocator.check( reinterpret_cast<Session *>( ptr_session ) ) );
    if ( not session->push_channel ) {
        session->push_channel = this;

        String s = session->data_to_push.str();
        if ( s.size() ) {
            out << s;
            session->rq_chan();
        } else
            loop->add_timeout( 30, session );
    }
}

void HttpRequest_Public::rq_chan_and_close() {
    oun << "FileSystem._insts[ " << session->num_inst << " ].make_channel();";
    cmd_end();
}
