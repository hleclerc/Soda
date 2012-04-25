#include <Celo/Listener_Factory.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Signal_WO.h>
#include <Celo/Timer_WO.h>
#include <Celo/Idle_WO.h>
#include <auto_ptr.h>
#include <signal.h>
#include <time.h>

#include "../Database/RootSession.h"
#include "HttpRequest_Supervision.h"
#include "HttpRequest_Public.h"
#include "SodaLoop.h"

SodaLoop::SodaLoop() : update_dl( this ) {
    #include <PrepArg/constructor.h>
}

int SodaLoop::run() {
    if ( verbose ) {
        #include <PrepArg/info.h>
    }

    std::auto_ptr<EventLoop  > auto_ptr_ev_loop ( ev_loop  = new EventLoop );
    std::auto_ptr<Database   > auto_ptr_database( database = new Database );
    root_session = database->session_allocator.factory( database, db_file );

    // fake listener (will be replaced by a generated dlopen'ed one)
    Listener_Factory<HttpRequest_Public,SodaLoop> f( port, this );
    *ev_loop << &f;
    listener = &f;


    // supervision
    Listener_Factory<HttpRequest_Supervision,SodaLoop> s( port_super, this );
    s.launch = launch_browser_super;
    *ev_loop << &s;

    // signal
    int sigs[] = { SIGINT, SIGQUIT, SIGKILL, -1 };
    *ev_loop << new Signal_WO<SodaLoop>( this, sigs );

    // timer
    *ev_loop << new Timer_WO<SodaLoop>( this, 0.5 );

    // idle
    *ev_loop << new Idle_WO<SodaLoop>( this );

    // launch update
    update();

    return ev_loop->run();
}

void SodaLoop::update() {
    update_dl.exec();
}

bool SodaLoop::idle() {
    database->end_round();
    return true;
}

bool SodaLoop::timeout() {
    // a new HttpRequest ?
    if ( update_dl.has_update ) {
        update_dl.has_update = false;
        update_dl.sy_req( listener );
        if ( launch_browser ) {
            listener->start_url = start_page;
            listener->launch_browser();
        }
    }

    // outdating channel
    for( TS::iterator iter = timeout_reqs.begin(); iter != timeout_reqs.end(); ++iter ) {
        PI64 o = time( 0 );
        while( true ) {
            JavascriptSession *s = iter->second.beg;
            if ( s and o >= s->date_channel ) {
                iter->second.pop_front();
                s->rq_chan_and_close_pc();
            } else
                break;
        }
    }

    return true;
}

bool SodaLoop::signal( int sig ) {
    std::cerr << "Sig " << sig << std::endl;
    return true;
}

void SodaLoop::add_timeout( PI64 delay, JavascriptSession *s ) {
    s->date_channel = time( 0 ) + delay;
    timeout_reqs[ delay ] << s;
}

void SodaLoop::rem_timeout( PI64 delay, JavascriptSession *s ) {
}
