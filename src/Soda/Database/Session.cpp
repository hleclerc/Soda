#include "../Model/Directory.h"
#include "../Sys/Stream.h"
#include "Database.h"
#include "Session.h"

Session::Session( Database *db, User *user ) : MP( this, db->root_dir ), db( db ), user( user ) {
    inactive = false;
    in_sod = false;

    default_watching_sessions = db->default_watching_sessions;
    default_watching_sessions << this;

    default_rights = db->right_set_list.no_Id;
    if ( user ) {
        default_rights.add( user, RD );
        default_rights.add( user, WR );
    }

    cwd << db->root_dir; ///< we start with '/'
    s = this; ///< for MP
}

void Session::on_change( Model *m ) {
}

void Session::end_round() {
}
