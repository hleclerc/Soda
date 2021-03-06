#ifndef HTTPREQUEST_PUBLIC_H
#define HTTPREQUEST_PUBLIC_H

#include <Celo/EventObj_HttpRequest.h>
#include <Celo/StreamSep.h>
#include <sstream>
#include <map>

#include "../Database/TmpModelMap.h"
#include "JavascriptSession.h"
#include "ServerLoop.h"
class Path;
class Ptr;

/**
*/
class HttpRequest_Public : public EventObj_HttpRequest {
public:
    HttpRequest_Public( int fd, ServerLoop *loop );
    virtual ~HttpRequest_Public();

    virtual int parse( char *beg, char *end ); ///< do nothing. Permits to instantiate a HttpRequest_Public
    virtual void hup(); ///< if closed

    void cmd_new_session( int num_inst ); ///< make a new session. num_inst is the number of the distant FileSystem instance
    void cmd_set_session( ST ptr_session ); ///<
    void cmd_creation( const String &type, ST tmp_id ); ///<
    void cmd_load_ptr( ST ptr_model, int num_callback ); ///<
    int  cmd_put_cnt( PT ptr_session, PT ptr_model, PT &length, const char *beg, const char *end ); //<
    void cmd_set_type( ST ptr_model, const String &type ); ///< set type for object creation
    void cmd_change( ST ptr_model, const String &data ); ///<
    void cmd_load( const String &path, int num_callback ); ///<
    int  cmd_put( PT ptr_session, PT ptr_model, PT &length, const char *beg, const char *end );
    int  cmd_end();

    int  send_all(); ///< send data in out
    int  end_put();

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

    Path *p;
    int put_fd;
    int end_num;
};

#endif // HTTPREQUEST_PUBLIC_H
