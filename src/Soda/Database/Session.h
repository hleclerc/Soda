#ifndef SESSION_H
#define SESSION_H

#include "../Sys/StringBlk.h"
#include "../Sys/Nstring.h"
#include "../Sys/Vec.h"
#include "SessionSet.h"
#include "RightSet.h"
#include <sstream>
class Directory;
class Database;
class BinInp;
class Model;
class User;
class Val;

/**
*/
class Session {
public:
    Session( Database *db, User *user );

    virtual void on_change( Model *m ); ///< what to do if m has changed during the previous round
    virtual void end_round(); ///<

    Model *factory( BinInp &inp, int t, RightSet rights );
    Model *factory( Nstring type );

    Model *operator[]( StringBlk path );

    //
    std::ostringstream errors; ///< hum

    SessionSet default_watching_sessions;
    RightSet default_rights;
    Vec<Model *> cwd; ///< current working directory
    Database *db; ///< originating database
    User *user;    

    Session *next_sod;
    bool in_sod; // in db->sod_list ?

    bool inactive;
};

#endif // SESSION_H
