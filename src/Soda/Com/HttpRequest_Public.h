#ifndef HTTPREQUEST_PUBLIC_H
#define HTTPREQUEST_PUBLIC_H

#include <Celo/EventObj_HttpRequest.h>
#include <Celo/StreamSep.h>
#include <sstream>
#include <map>

#include "../Database/TmpModelMap.h"
#include "JavascriptSession.h"
#include "ServerLoop.h"
class Ptr;

/**
*/
class HttpRequest_Public : public EventObj_HttpRequest {
public:
    HttpRequest_Public( int fd, ServerLoop *loop );
    virtual int parse( char *beg, char *end ); ///< do nothing. Permits to instantiate a HttpRequest_Public
    virtual void hup(); ///< if closed

    void cmd_new_session( int num_inst ); ///< make a new session. num_inst is the number of the distant FileSystem instance
    void cmd_set_session( ST ptr_session ); ///<
    void cmd_creation( const String &type, ST tmp_id ); ///<
    void cmd_load_ptr( ST ptr_model, int num_callback ); ///<
    void cmd_change( ST ptr_model, const String &data ); ///<
    void cmd_load( const String &path, int num_callback ); ///<
    int  cmd_end();

    void mk_chan( ST ptr_session ); ///< we want *this to be a push channel
    void rq_chan_and_close(); /// request for a channnel (to session)

    // helpers for output
    StreamSepMaker<std::ostringstream> oun;
    std::ostringstream out;

    // context
    Vec<Ptr *> ptr_to_update;
    TmpModelMap tmp_map;
    ServerLoop *loop;
    Session *session;
};

#endif // HTTPREQUEST_PUBLIC_H
