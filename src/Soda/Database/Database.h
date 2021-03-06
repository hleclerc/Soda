#ifndef DATABASE_H
#define DATABASE_H

#include "../Sys/NstringList.h"
#include "../Sys/SecureAlloc.h"
#include "../Model/Model.h"
#include "Session.h"
class Directory;

/**

*/
class Database {
public:
    Database( const char *db_directory );
    ~Database();

    Model *add_to_new_mod_list( Model *m, Session *s, Nstring type ); ///< when a new Model is created
    void reg_type( StringBlk type, Session *s, int c );
    Model *add_to_mod_list( Model *m, Session *s );
    Session *add_to_sod_list( Session *s );
    void remove_refs_to( Session *s );
    String new_file( User *user );
    void end_round();

    SessionSet default_watching_sessions;
    SessionSet::List session_set_list;
    RightSet::List right_set_list;
    SecureAlloc session_allocator;
    SecureAlloc model_allocator;
    NstringList nstring_list;
    const char *db_directory; ///< where the files are created and stored
    Directory *root_dir; ///< the "/" directory
    Session *sod_list; ///< list of sessions that have something to send
    Model *mod_list; ///< last object that has been modified during the prededing round(s)
    User *root_usr;

    struct SessionAndCallback {
        Session *session;
        int callback;
    };

    std::map<Nstring,Vec<SessionAndCallback> > reg_types;
};

#endif // DATABASE_H
