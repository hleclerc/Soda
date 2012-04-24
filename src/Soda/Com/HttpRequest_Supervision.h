#ifndef HTTPREQUEST_SUPERVISION_H
#define HTTPREQUEST_SUPERVISION_H

#include <Celo/EventObj_HttpRequest.h>
#include <Celo/EventLoop.h>
#include "SodaLoop.h"

/**
*/
class HttpRequest_Supervision : public EventObj_HttpRequest {
public:
    HttpRequest_Supervision( int fd, SodaLoop *loop );
    void index();

    #define SIPE_CHARP char *
    #define SIPE_CLASS
    #include "HttpRequest_Supervision_parser.h"
    #undef  SIPE_CHARP
    #undef  SIPE_CLASS

    SodaLoop *loop;
};

#endif // HTTPREQUEST_SUPERVISION_H
