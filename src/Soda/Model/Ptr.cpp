#include "../Database/TmpModelMap.h"
#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"
#include "Ptr.h"

Ptr::Ptr( RightSet rights, SessionSet watching_sessions, SI64 man ) : Model( rights, watching_sessions ), man( man ) {
}

Ptr::Ptr( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    man = inp->read();
}

Ptr::operator SI64() const {
    return man;
}

void Ptr::write_str( Stream &out ) const {
    out << man;
}

void Ptr::write_dmp( BinOut &out ) const {
    out << man;
}

bool Ptr::write_ujs( Stream &nut, Stream &uut, Session * ) const {
    uut << "FileSystem._objects[ " << this << " ].set( " << man << " );\n";
    return true;
}

Nstring Ptr::type() const {
    return NSTRING_Ptr;
}

int Ptr::type_dump() const {
    return _Ptr;
}

bool Ptr::_set( const TmpModelMap &mm, StringBlk data ) {
    SI64 oman = man;

    man = 0;
    for( int i = 0; i < data.size(); ++i ) {
        if ( data[ i ] < '0' or data[ i ] > '9' )
            break;
        man = 10 * man + ( data[ i ] - '0' );
    }

    //
    if ( man & 3 )
        man = (ST)mm[ man ];

    return oman != man;
}

void Ptr::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];

    Model *p = map_read[ get_model() ];
    set_model( p );

    if ( p )
        p->map_ptr( map_read );
}

