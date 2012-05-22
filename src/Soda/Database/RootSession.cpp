#include "../Model/Directory.h"
#include "../Model/User.h"

#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"

#include "RootSession.h"
#include "Database.h"
#include "MapRead.h"

#include <fcntl.h>

int RootSession::reading_with_file_version = RootSession::FILE_VERSION;

RootSession::RootSession( Database *db, const char *db_filename ) : Session( db, 0 ) {
    //
    if ( db_filename )
        _load( db_filename );

    // root user
    if ( not db->root_usr ) {
        db->root_usr = db->model_allocator.factory( default_rights, default_watching_sessions );
        db->add_to_mod_list( db->root_usr, this );
    }

    default_rights.add( db->root_usr, RD );
    default_rights.add( db->root_usr, WR );
    user = db->root_usr;

    // root directory
    if ( not db->root_dir ) {
        db->root_dir = db->model_allocator.factory( default_rights, default_watching_sessions );
        db->add_to_mod_list( db->root_dir, this );
    }

    // (re-) open dump file. add basic info
    _dump_fd = open( db_filename, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0777 );
    _dump << (int)Model::_DbFileVersion << (int)FILE_VERSION;
    _dump << (int)Model::_RootDir << db->root_dir;
    _dump << (int)Model::_RootUsr << db->root_usr;
}

void RootSession::on_change( Model *m ) {
    _dump << m->type_dump();
    _dump << m->rights;
    _dump << m;
    m->write_dmp( _dump );
}

void RootSession::end_round() {
    // dump new Nstrings
    for( const NstringList::Item *n = db->nstring_list.lmod; n; n = n->pmod ) {
        _dump << (int)Model::_Nstring;
        _dump << n;
        n->write_dmp( _dump );
    }
    db->nstring_list.lmod = 0;

    // dump new Rights
    for( const RightSet::Item *n = db->right_set_list.lmod; n; n = n->pmod ) {
        _dump << (int)Model::_Rights;
        _dump << n;
        n->write_dmp( _dump );
    }
    db->right_set_list.lmod = 0;
}

void RootSession::flush() {
    if ( _dump.size() ) {
        write( _dump_fd, _dump.data(), _dump.size() );
        _dump.clear( 1024 );
    }
}

void RootSession::_load( const char *db_filename ) {
    Vec<std::pair<RightSet,RightSet::Item> > d_map;
    BinInp inp( db_filename );
    MapRead map_read;

    while ( true ) {
        int t = inp.read();
        if ( not inp ) break;

        // PRINT( t );
        if ( t == Model::_DbFileVersion ) {
            reading_with_file_version = inp.read();
        } else if ( t == Model::_RootDir ) {
            db->root_dir = inp.read();
        } else if ( t == Model::_RootUsr ) {
            db->root_usr = inp.read();
        } else if ( t == Model::_Nstring ) {
            Nstring p = inp.read();
            map_read.n_map[ p ] = db->nstring_list( inp );
        } else if ( t == Model::_Rights ) {
            std::pair<RightSet,RightSet::Item> m;
            m.first = inp.read();
            m.second = RightSet::Item( inp, &db->right_set_list );
            d_map << m;
        } else {
            RightSet rights = inp.read();
            Model *p = inp.read();
            map_read.m_map[ p ] = factory( inp, t, rights );
        }
    }

    // Rights -> Rights
    for( int i = 0; i < d_map.size(); ++i ) {
        // User * -> User *
        RightSet::Item &d = d_map[ i ].second;
        for( unsigned j = 0; j < d._n; ++j )
            for( unsigned k = 0; k < d.data[ j ].size(); ++k )
                d.data[ j ][ k ] = dynamic_cast<User *>( map_read.m_map[ d.data[ j ][ k ] ] );

        // Rights using correct User *
        map_read.r_map[ d_map[ i ].first ] = db->right_set_list.find( d.data );
    }

    // MapRead -> MapRead
    if ( db->root_usr ) db->root_usr = dynamic_cast<User *>( map_read[ db->root_usr ] );
    if ( db->root_dir ) db->root_dir = dynamic_cast<Directory *>( map_read[ db->root_dir ] );

    ++Model::cur_op_id;
    if ( db->root_dir ) db->root_dir->map_ptr( map_read );
}
