#include "Database.h"
#include "../Model/Val.h"

Database::Database() {
    session_set_list.extra = this;
    sod_list = 0;
    root_dir = 0;
    root_usr = 0;
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

Session *Database::add_to_sod_list( Session *s ) {
    if ( not s->in_sod ) {
        s->in_sod = true;
        s->next_sod = sod_list;
        sod_list = s;
    }
    return s;
}

void Database::end_round() {
    // modified models
    for( Model *m = mod_list; m; m = m->next_mod ) {
        m->rm_inactive_sessions();
        for( int i = 0; i < m->watching_sessions.size(); ++i ) {
            Session *s = add_to_sod_list( m->watching_sessions[ i ] );
            s->on_change( m );
        }

        m->mod_session = 0;
    }
    mod_list = 0;

    // sessions that have something to do
    for( Session *s = sod_list; s; s = s->next_sod )
        s->end_round();
    sod_list = 0;
}


