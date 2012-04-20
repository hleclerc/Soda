#include "NstringList.h"
#include "NstringHash.h"
#include "BinInp.h"

#include <stdlib.h>
#include <string.h>

#define US( N ) Nstring NSTRING_##N;
#include "UsualStrings_Decl.h"
#undef US

NstringList::NstringList() : lmod( 0 ), nb_nstrings( 0 ) {
    for( int i = 0; i < size_hash_list; ++i )
        hash_list[ i ] = 0;

    #define US( N ) NSTRING_##N = operator()( #N );
    #include "UsualStrings_Decl.h"
    #undef US
}

Nstring NstringList::operator()( StringBlk s ) {
    return operator()( s.c_str(), s.size() );
}

Nstring NstringList::operator()( const char *data, int size, unsigned hash_val ) {
    if ( not size )
        return Nstring();
    hash_val %= size_hash_list;

    // look in hash_list
    for( Item *i = hash_list[ hash_val ]; i; i = i->prev )
        if ( i->size == size and strncmp( i->data, data, size ) == 0 )
            return i;

    // not found ? -> create a new one
    int n = nb_nstrings++;
    Item *i = new Item;
    i->prev      = hash_list[ hash_val ];
    i->size      = size;
    hash_list[ hash_val ] = i;

    // inverse chronological order
    i->pmod = lmod;
    lmod = i;

    // copy data
    i->data = (char *)malloc( size + 1 );
    memcpy( i->data, data, size );
    i->data[ size ] = 0;

    return i;
}

Nstring NstringList::operator()( const char *beg, int len ) {
    return operator()( beg, len, nstring_hash( beg, len ) );
}

Nstring NstringList::operator()( const char *beg ) {
    return operator()( beg, strlen( beg ) );
}

Nstring NstringList::operator()( BinInp &inp ) {
    int size = inp.read();
    char *_ata = new char[ ceil( size, 4 ) ];
    inp.read( _ata, ceil( size, 4 ) );
    Nstring res = operator()( _ata, size );
    delete [] _ata;
    return res;
}

int NstringList::size() const {
    return nb_nstrings;
}
