#include "../Sys/Stream.h"
#include "RootSession.h"
#include "Directory.h"
#include "Database.h"

int RootSession::reading_with_file_version = RootSession::FILE_VERSION;

RootSession::RootSession( Database *db, const char *db_filename ) : Session( db, 0 ) {
    //
    if ( db_filename )
        _load( db_filename );

    //
    if ( not db->root_dir ) {
        db->root_dir = db->model_allocator.factory( default_rights, default_watching_sessions );
        db->add_to_mod_list( db->root_dir, this );
        m = db->root_dir; // because this is a child of MP
    }

    //
    //    if ( not user ) {
    //    }

    //
    dump.open( db_filename, BinOut::App() );
}

void RootSession::on_change( Model *m ) {
    dump << m->type_dump();
    m->write_dmp( dump );
}

void RootSession::_load( const char *db_filename ) {
}
