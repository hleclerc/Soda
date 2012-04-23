#ifndef SODALOOP_H
#define SODALOOP_H

#define PREPARG_FILE <Soda/args.h>

#include <PrepArg/default_values_stl.h>
#include "../Database/Database.h"
#include "../InstallDir.h"
#include "../Sys/FList.h"
#include "UpdateDl.h"

class Listener_WithLaunch;
class JavascriptSession;
class RootSession;
class EventLoop;

/**
*/
class SodaLoop {
public:
    SodaLoop();
    int run();
    void update();

    bool idle();            ///<
    bool timeout();         ///<
    bool signal( int sig ); ///<

    void add_timeout( PI64 delay, JavascriptSession *s );
    void rem_timeout( PI64 delay, JavascriptSession *s );

    typedef std::map<double,FList<JavascriptSession> > TS;
    #include <PrepArg/attributes.h>
    Listener_WithLaunch *listener;
    EventLoop           *ev_loop;
    UpdateDl             update_dl;
    RootSession         *root_session;
    Database            *database;
    TS                   timeout_reqs; ///< [ delay ] -> list
};

#endif // SODALOOP_H
