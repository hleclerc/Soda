#ifndef SODAREQUEST_PUBLIC_H
#define SODAREQUEST_PUBLIC_H

#include "../Database/TmpModelMap.h"
#include <Celo/EventObj_WO.h>
#include "../Sys/BinOut.h"
#include "../Sys/Stream.h"
#include "../Sys/Vec.h"
#include <iostream>
class SodaSession;
class ServerLoop;
class Session;
class Model;

/**
*/
class SodaRequest_Public : public EventObj_WO {
public:
    SodaRequest_Public( int fd, ServerLoop *loop );
    virtual ~SodaRequest_Public();

    virtual bool cnt_default_value() const;
    void send(); ///< send data in b

    BinOut b;
protected:
    void cmd_update_PI32( PI64 m, SI32 info ); ///<
    void cmd_update_PI64( PI64 m, PI64 info ); ///<
    void cmd_update_PI8 ( PI64 m, PI8  info ); ///<
    void cmd_update_6432( PI64 m, SI64 man, SI32 exp ); ///<
    void cmd_update_cstr( PI64 m, const char *type_str, int type_len ); ///<
    void cmd_push_string( const char *str, int len ); ///<
    void cmd_push_model ( PI64 m ); ///<

    void cmd_creation( PI64 tmp_id, const char *type_str, int type_len, const char *type_stu, int type_leu );
    void cmd_creation( PI64 tmp_id, const char *type_str, int type_len );
    void cmd_reg_type( int n_callback, const char *type_str, int type_len );
    void cmd_load_ptr( int n_callback, PI64 model_id );
    void cmd_load( int n_callback, const char *path_str, int path_len );
    void cmd_end();

    #define SIPE_CHARP char *
    #define SIPE_CLASS
    #include "SodaRequest_Public_parser.h"

    TmpModelMap  tmp_map;
    SodaSession *session;
    ServerLoop  *loop;
    Vec<Model *> model_stack;
    Vec<String > string_stack;
};

#endif // SODAREQUEST_PUBLIC_H
