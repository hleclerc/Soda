#include "SodaRequest_Public.h"
#include "ServerLoop.h"

#include "../Database/Session.h"
#include "../Sys/BinOutGen.h"
#include "../Sys/Stream.h"

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ), loop( loop ) {
    session = loop->database->session_allocator.factory( loop->database, loop->database->root_usr );
}

void SodaRequest_Public::cmd_load( int n_callback, char *path_str, int path_len ) {
    if ( session and session->user ) {
        if ( Model *m = session->operator[]( StringBlk( path_str, path_len ) ) ) {
            if ( m->rights.has( session->user, RD ) ) {
                //
                std::ostringstream os;
                BinOutGen<std::ostringstream> b( os );

                b << 'L' << n_callback << (SI64)m;

                String res = os.str();
                char *data = (char *)res.data();
                int size = res.size();
                send_str( data, size );
            }
        }
    }
}

