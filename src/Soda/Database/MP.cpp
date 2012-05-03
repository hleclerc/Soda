#include "../Sys/UsualStrings.h"
#include "../Sys/BinInp.h"

#include "../Model/ModelWithAttrAndName.h"
#include "../Model/Directory.h"
#include "../Model/File.h"
#include "../Model/User.h"
#include "../Model/Val.h"
#include "../Model/Str.h"

#include "Session.h"
#include "Database.h"
#include "MP.h"

MP::MP( Session *s ) : s( s ), m( s and s->cwd.size() ? s->cwd.back() : 0 ) {}

void MP::write_str( Stream &out ) const {
    if ( m and not ( b or n ) )
        m->write_str( out );
    else
        out << "(void)";
}

MP MP::operator[]( StringBlk path ) {
    if ( not s )
        return *this;

    // get base dir
    Model *d = m;
    if ( path.size() and path[ 0 ] == '/' ) {
        d = s->db->root_dir;
        ++path;
    }

    // get dirs in $path
    while ( true ) {
        StringBlk p = path.split( '/' );
        if ( path ) {
            if ( Model *m = d->attr( p ) ) {
                d = m;
                if ( not d->rights.has( s->user, RD ) ) {
                    s->errors << "Attribute does not exist";
                    return MP();
                }
            } else {
                s->errors << "Attribute does not exist";
                return MP();
            }
        } else {
            path = p;
            break;
        }
    }

    // path with a '/' at the end ? (as in e.g. "path/")
    if ( not path )
        return MP( s, d );

    // m->rights.has( session->user, RD );
    // pointer on an object that already exists
    if ( Model *r = d ? d->attr( path ) : 0 )
        return MP( s, r );

    return MP( s, d, path );
}

MP MP::operator[]( Nstring path ) {
    if ( s == 0 or m == 0 or not path )
        return *this;

    if ( Model *r = m->attr( path ) )
        return MP( s, r );
    return MP( s, m, path );
}

Model *MP::factory( Nstring type ) {
    #define DECL( M ) if ( type == NSTRING_##M ) { \
        M *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions ); \
        return s->db->add_to_mod_list( res, s ); \
    }
    #include "../Model/Model_decl.h"
    #undef DECL

    // else -> ModelWithAttrAndName
    ModelWithAttrAndName *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions, type );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( BinInp &inp, int type_dump, RightSet rights, SessionSet watching_sessions ) {
    switch ( type_dump ) {
    #define DECL( M ) case Model::_##M: return s->db->add_to_mod_list( (M *)s->db->model_allocator.factory( &inp, rights, watching_sessions ), s );
    #include "../Model/Model_decl.h"
    DECL( ModelWithAttrAndName )
    #undef DECL
    }
    return 0;
}

Model *MP::factory( New_Directory ) {
    if ( not s ) return 0;
    Directory *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( const std::string &val ) {
    if ( not s ) return 0;
    Str *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions, val );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( New_File nf ) {
    if ( not s ) return 0;
    // void File object
    File *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions );
    s->db->add_to_mod_list( res, s );
    // fill the attributes
    MP mp( s, res );
    mp[ NSTRING_name  ] = nf.name;
    mp[ NSTRING__ptr  ] = (SI64)nf.p;
    mp[ NSTRING__info ].create( new_Obj( NSTRING_Model ) );
    return res;
}

Model *MP::factory( StringBlk val ) {
    return factory( std::string( val.c_str(), val.c_str() + val.size() ) );
}

Model *MP::factory( New_OS nf ) {
    if ( not s ) return 0;
    ModelWithAttrAndName *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions, s->db->nstring_list( nf.name ) );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( New_ON nf ) {
    if ( not s ) return 0;
    ModelWithAttrAndName *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions, nf.name );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( SI64 val ) {
    if ( not s ) return 0;
    Val *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions, val );
    return s->db->add_to_mod_list( res, s );
}

Model *MP::factory( New_Lst ) {
    if ( not s ) return 0;
    Lst *res = s->db->model_allocator.factory( s->default_rights, s->default_watching_sessions );
    return s->db->add_to_mod_list( res, s );
}


User *MP::_user() {
    return s ? s->user : 0;
}

MP MP::_create_attr( Model *p ) {
    if ( p ) {
        if ( b ) { // we have to create a new model from a StringBlk ?
            if ( m and m->rights.has( s->user, WR ) and m->_add_attr( s, b, p ) )
                s->db->add_to_mod_list( m, 0 );
        } else if ( n ) { // we have to create a new model for a Nstring ?
            if ( m and m->rights.has( s->user, WR ) and m->_add_attr( s, n, p ) )
                s->db->add_to_mod_list( m, 0 );
        }
    }
    return MP( s, p );
}

void MP::_add_to_mod_list( Model *m ) {
    s->db->add_to_mod_list( m, s );
}

