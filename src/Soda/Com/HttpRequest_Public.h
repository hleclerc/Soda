#ifndef HTTPREQUEST_PUBLIC_H
#define HTTPREQUEST_PUBLIC_H

#include <Celo/EventObj_HttpRequest.h>
#include <Celo/StreamSep.h>
#include <sstream>
#include <map>

#include "../Database/TmpModelMap.h"
#include "JavascriptSession.h"
#include "SodaLoop.h"

/**
*/
class HttpRequest_Public : public EventObj_HttpRequest {
public:
    HttpRequest_Public( int fd, SodaLoop *loop );
    virtual int parse( char *beg, char *end ); ///< do nothing. Permits to instantiate a HttpRequest_Public
    virtual void hup(); ///< if closed

    void cmd_new_session( int num_inst ); ///< make a new session. num_inst is the number of the distant FileSystem instance
    void cmd_set_session( ST ptr_session ); ///<
    void cmd_creation( const String &type, ST tmp_id, const String &data ); ///<
    void cmd_change( ST ptr_model, const String &data ); ///<
    void cmd_load( const String &path, int num_callback ); ///<
    void cmd_save( const String &path, ST ptr_model ); ///<
    int  cmd_end();

    void mk_chan( ST ptr_session ); ///< we want *this to be a push channel
    void rq_chan(); /// request for a channnel (to session)

    // helpers for output
    StreamSepMaker<std::ostringstream> oun;
    std::ostringstream out;

    // context
    JavascriptSession *session;
    TmpModelMap tmp_map;
    SodaLoop *loop;
};

#endif // HTTPREQUEST_PUBLIC_H
