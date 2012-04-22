#include "../Sys/UsualStrings.h"
#include "Directory.h"
#include "Database.h"
#include "File.h"
#include "MP.h"

Directory::Directory( RightSet rights, SessionSet watching_sessions ) : Lst( rights, watching_sessions  ) {
}

Nstring Directory::type() const {
    return NSTRING_Directory;
}

Model *Directory::attr( Nstring name ) const {
    return attr( StringBlk( name ) );
}

Model *Directory::attr( StringBlk name ) const {
    for( int i = 0; i < _data.size(); ++i )
        if ( Model *f = _data[ i ] )
            if ( Model *n = f->attr( NSTRING_name ) )
                if ( n->equal( name ) )
                    if ( Model *p = f->attr( NSTRING__ptr ) )
                        return db()->model_allocator.check( (Model *)p->operator SI64() );
    return 0;
}

bool Directory::_add_attr( Session *s, StringBlk name, Model *m ) {
    if ( not name )
        return false;

    // already in data ?
    for( int i = 0; i < _data.size(); ++i )
        if ( Model *f = _data[ i ] )
            if ( Model *n = f->attr( NSTRING_name ) )
                if ( n->equal( name ) )
                    return false;


    // else, create a new file
    MP d( s, this );
    d << new_File( name, m );
}

bool Directory::_add_attr( Session *s, Nstring name, Model *m ) {
    return _add_attr( s, StringBlk( name ), m );
}
