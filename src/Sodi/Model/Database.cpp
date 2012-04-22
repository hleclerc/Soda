#include "Database.h"
#include "Val.h"

Database::Database() {
    session_set_list.extra = this;
    sod_list = 0;
    root_dir = 0;
    mod_list = 0;
}

Database::~Database() {
}

Model *Database::add_to_mod_list( Model *m, Session *s ) {
    if ( not m->mod_session ) {
        m->mod_session = s;
        m->next_mod = mod_list;
        mod_list = m;
    }
    return m;
}

void Database::end_round() {
    // modified models
    for( Model *m = mod_list; m; m = m->next_mod ) {
        for( int i = 0; i < m->watching_sessions.size(); ++i )
            m->watching_sessions[ i ]->on_change( m );

        m->mod_session = 0;
    }
    mod_list = 0;

    // sessions that have something to do
    sod_list = 0;
}


