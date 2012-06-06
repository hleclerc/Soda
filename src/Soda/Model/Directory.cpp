#include "../Database/Database.h"
#include "../Sys/UsualStrings.h"

#include "Directory.h"
#include "File.h"

Directory::Directory( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Lst( inp, rights, watching_sessions  ) {
}

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
