#ifndef SODAREQUEST_PUBLIC_H
#define SODAREQUEST_PUBLIC_H

#include <Celo/EventObj_WO.h>
class ServerLoop;

/**
*/
class SodaRequest_Public : public EventObj_WO {
public:
    SodaRequest_Public( int fd, ServerLoop *loop );

protected:
    virtual int parse( char *beg, char *end );

    ServerLoop *loop;
};

#endif // SODAREQUEST_PUBLIC_H
