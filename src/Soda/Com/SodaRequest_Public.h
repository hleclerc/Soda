#ifndef SODAREQUEST_PUBLIC_H
#define SODAREQUEST_PUBLIC_H

#include <Celo/EventObj_WO.h>
#include "../Sys/BinOut.h"
#include <iostream>
class ServerLoop;
class Session;

/**
*/
class SodaRequest_Public : public EventObj_WO {
public:
    SodaRequest_Public( int fd, ServerLoop *loop );

protected:
    void cmd_load( int n_callback, const char *path_str, int path_len );
    void cmd_end();

    #define SIPE_CHARP char *
    #define SIPE_CLASS
    #include "SodaRequest_Public_parser.h"

    Session *session;
    ServerLoop *loop;
    BinOut b;
};

#endif // SODAREQUEST_PUBLIC_H
