#include "../Sys/UsualStrings.h"
#include "../Model/Directory.h"
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
    std::map<Nstring,Vec<SessionAndCallback> >::const_iterator iter = reg_types.find( type );
    if ( iter != reg_types.end() ) {
        for( int i = 0; i < iter->second.size(); ++i ) {
            Session *s = add_to_sod_list( iter->second[ i ].session );
            s->on_reg_type( m, iter->second[ i ].callback );
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

struct FindObjOfType : Model::Sweeper {
    virtual void operator()( Model *m ) {
        if ( m->type() == n )
            res.insert( m );
    }
    std::set<Model *> res;
    Nstring n;
};

void Database::reg_type( StringBlk type, Session *s, int c ) {
    FindObjOfType foot;
    foot.n = nstring_list( type );

    SessionAndCallback sc;
    sc.session = s;
    sc.callback = c;
    reg_types[ foot.n ] << sc;

    // model of type $type already present in the database
    root_dir->sweep( foot );
    if ( foot.res.size() ) {
        add_to_sod_list( s );
        for( std::set<Model *>::iterator iter = foot.res.begin(); iter != foot.res.end(); ++iter )
            s->on_reg_type( *iter, c );
    }
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

void Database::remove_refs_to( Session *s ) {
    for( std::map<Nstring,Vec<SessionAndCallback> >::iterator iter = reg_types.begin(); iter != reg_types.end(); ++iter )
        for( int i = 0; i < iter->second.size(); ++i )
            if ( iter->second[ i ].session == s )
                iter->second.remove( i-- );
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

