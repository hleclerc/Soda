#ifndef UPDATEDL_H
#define UPDATEDL_H

#include <Celo/Thread.h>
#include <Celo/Mutex.h>
#include "../Sys/Command.h"

class Listener_WithLaunch;
class SodaLoop;

/**
*/
class UpdateDl : public Thread {
public:
    typedef void TF( Listener_WithLaunch *res );

    UpdateDl( SodaLoop *l );
    virtual void run();

    // input
    SodaLoop   *l;

    // helpers
    Mutex       mutex;      ///< we don't to compile twice the same things

    // output
    bool        has_update; ///< set after a successfull update
    void       *dl_req;        ///< resulting lib
    TF         *sy_req;     ///<
    int         ret;        ///< error code if any
    std::string date;       ///< date last construction
    std::string dl_error;   ///<

    Command     sipe_cmd;   ///<
    Command     comp_cmd;   ///< .cpp  -> .o
    Command     mobj_cmd;   ///< .data -> .o
    Command     link_cmd;   ///< [ .o ] -> .so
};


#endif // UPDATEDL_H
