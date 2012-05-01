#include "SodaRequest_Public.h"
#include <unistd.h>

SodaRequest_Public::SodaRequest_Public( int fd, ServerLoop *loop ) : EventObj_WO( fd ) {
}

int SodaRequest_Public::parse( char *beg, char *end ) {
    write( 0, beg, end - beg );
    return CNT;
}
