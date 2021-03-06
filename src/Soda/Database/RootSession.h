#ifndef ROOTSESSION_H
#define ROOTSESSION_H

#include "../Sys/BinOut.h"
#include "Session.h"

/**
  Session for tha soda server.

  It permits to:
    - initialize the database (load or "auto" init)
    - dump modifications
*/
class RootSession : public Session {
public:
    enum { FILE_VERSION = 1 };

    RootSession( Database *db, const char *db_filename = 0 );
    virtual void on_change( Model *m );
    virtual void end_round();

    void flush(); ///< flush dump

    // dump IO
    static int reading_with_file_version; ///< readers must take this value into account if changes in the binary format since the production of .db file

protected:
    void _load( const char *db_filename );

    int    _dump_fd;
    BinOut _dump; ///<
};

#endif // ROOTSESSION_H
