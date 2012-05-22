#ifndef SODAREQUEST_PUBLIC_H
#define SODAREQUEST_PUBLIC_H

#include <Celo/EventObj_WO.h>
#include "../Sys/BinOut.h"
#include <iostream>
class SodaSession;
class ServerLoop;
class Session;

/**
*/
class SodaRequest_Public : public EventObj_WO {
public:
    SodaRequest_Public( int fd, ServerLoop *loop );

    void send(); ///< send data in b
    BinOut b;
protected:
    void cmd_reg_type( int n_callback, const char *type_str, int type_len );
    void cmd_load_ptr( int n_callback, PI64 ptr );
    void cmd_load( int n_callback, const char *path_str, int path_len );
    void cmd_end();

    #define SIPE_CHARP char *
    #define SIPE_CLASS
    #include "SodaRequest_Public_parser.h"

    SodaSession *session;
    ServerLoop *loop;
};

#endif // SODAREQUEST_PUBLIC_H
