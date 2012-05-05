#include "../Model/ModelWithAttrAndName.h"
#include "../Model/Directory.h"
#include "../Model/File.h"
#include "../Model/User.h"
#include "../Model/Val.h"
#include "../Model/Str.h"

#include "../Sys/UsualStrings.h"
#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"

#include "Database.h"
#include "Session.h"

Session::Session( Database *db, User *user ) : db( db ), user( user ) {
    inactive = false;
    in_sod = false;

    default_watching_sessions = db->default_watching_sessions;
    default_watching_sessions << this;

    default_rights = db->right_set_list.no_Id;
    if ( user ) {
        default_rights.add( user, RD );
        default_rights.add( user, WR );
    }

    cwd << db->root_dir; ///< we start with '/' as current directory
}

void Session::on_change( Model *m ) {
}

void Session::end_round() {
}

Model *Session::factory( BinInp &inp, int t, RightSet rights ) {
    switch ( t ) {
    #define DECL( M ) case Model::_##M: return db->add_to_mod_list( (M *)db->model_allocator.factory( &inp, rights, default_watching_sessions ), this );
    #include "../Model/Model_decl.h"
    DECL( ModelWithAttrAndName )
    #undef DECL
    }
    return 0;
}

Model *Session::factory( Nstring type ) {
    #define DECL( M ) if ( type == NSTRING_##M ) { \
        M *res = db->model_allocator.factory( default_rights, default_watching_sessions ); \
        return db->add_to_mod_list( res, this ); \
    }
    #include "../Model/Model_decl.h"
    #undef DECL

    // else -> ModelWithAttrAndName
    ModelWithAttrAndName *res = db->model_allocator.factory( default_rights, default_watching_sessions, type );
    return db->add_to_mod_list( res, this );
}

Model *Session::operator[]( StringBlk path ) {
    // basic checks
    if ( not user )
        return 0;

    // get base dir
    Model *d;
    if ( path.size() and path[ 0 ] == '/' ) {
        d = db->root_dir;
        ++path;
    } else if ( cwd.size() )
        d = cwd.back();
    else
        return 0;

    // get dirs in $path
    while ( StringBlk p = path.split( '/' ) ) {
        if ( p.size() ) {
            d = d->attr( p );
            if ( not d )
                return d;
        }
    }

    return d;
}

