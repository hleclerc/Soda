#ifndef ROOTSESSION_H
#define ROOTSESSION_H

#include "../Sys/BinOut.h"
#include "Session.h"

/**
*/
class RootSession : public Session {
public:
    enum { FILE_VERSION = 1 };

    RootSession( Database *db, const char *db_filename = 0 );
    virtual void on_change( Model *m ); ///<

    // dump IO
    static int reading_with_file_version; ///< readers must take this value into account if changes in the binary format since the production of .db file
    BinOut dump; ///<

protected:
    void _load( const char *db_filename );
};

#endif // ROOTSESSION_H
