#ifndef SESSION_H
#define SESSION_H

#include "../Sys/Vec.h"
#include "SessionSet.h"
#include "RightSet.h"
#include "MP.h"
#include <sstream>
class Directory;
class Database;
class BinInp;
class Model;
class User;

/**
*/
class Session : public MP {
public:
    Session( Database *db, User *user );
    virtual void on_change( Model *m ); ///<

    std::ostringstream errors; ///< hum

    SessionSet default_watching_sessions;
    RightSet default_rights;
    Session *next_sod;
    Vec<Model *> cwd;
    Database *db;
    User *user;
};

#endif // SESSION_H
