#include "../Sys/UsualStrings.h"
#include "../Model/User.h"
#include "Database.h"
#include <sys/stat.h>

Database::Database( const char *db_directory ) : db_directory( db_directory ) {
    session_set_list.extra = this;
    sod_list = 0;
    root_dir = 0;
    root_usr = 0;
    mod_list = 0;

    default_watching_sessions = session_set_list.no_Id;

    // bulk directory
    if ( db_directory )
        mkdir( db_directory, 0777 );
}

Database::~Database() {
}

Model *Database::add_to_new_mod_list( Model *m, Session *s, Nstring type ) {
    std::map<Nstring,Vec<Session *> >::const_iterator iter = reg_types.find( type );
    PRINT( type );
    if ( iter != reg_types.end() ) {
        for( int i = 0; i < iter->second.size(); ++i ) {
            Session *s = add_to_sod_list( iter->second[ i ] );
            s->on_reg_type( m );
        }

    }
    return add_to_mod_list( m, s );
}

Model *Database::add_to_mod_list( Model *m, Session *s ) {
    if ( not m->in_mod_list ) {
        m->in_mod_list = true;
        m->mod_session = s;
        m->next_mod = mod_list;
        mod_list = m;
    }
    return m;
}

Session *Database::add_to_sod_list( Session *s ) {
    if ( not s->in_sod ) {
        s->in_sod = true;
        s->next_sod = sod_list;
        sod_list = s;
    }
    return s;
}

void Database::reg_type( StringBlk type, Session *s ) {
    reg_types[ nstring_list( type ) ] << s;
}

void Database::end_round() {
    // modified models
    for( Model *m = mod_list; m; m = m->next_mod ) {
        m->rm_inactive_sessions();
        for( int i = 0; i < m->watching_sessions.size(); ++i ) {
            Session *s = add_to_sod_list( m->watching_sessions[ i ] );
            s->on_change( m );
        }

        m->in_mod_list = false;
        m->mod_session = 0;
    }
    mod_list = 0;

    // sessions that have something to do
    for( Session *s = sod_list; s; s = s->next_sod ) {
        s->end_round();

        s->in_sod = false;
    }
    sod_list = 0;
}


String Database::new_file( User *user ) {
    // base
    std::ostringstream res;
    if ( db_directory )
        res << db_directory << "/";
    if ( user )
        if ( Model *n = user->attr( NSTRING_name ) )
            res << n->operator String() << "/";

    // + random number
    while ( true ) {
        std::ostringstream tmp;
        tmp << res.str() << rand();

        struct stat buf;
        if ( stat( tmp.str().c_str(), &buf ) )
            return tmp.str();
    }
}

