#include "../Sys/Stream.h"
#include "Directory.h"
#include "Database.h"
#include "Session.h"

Session::Session( Database *db, User *user ) : MP( this, db->root_dir ), db( db ), user( user ) {
    default_watching_sessions = db->session_set_list.no_Id;
    default_watching_sessions << this;

    default_rights = db->right_set_list.no_Id;
    default_rights.add( user, RightSet::RD );
    default_rights.add( user, RightSet::WR );

    cwd << db->root_dir; ///< we start with '/'
    s = this; ///< for MP
}

void Session::on_change( Model *m ) {
}
