#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"
#include "Val.h"

#include <limits>
#include <cmath>

Val::Val( RightSet rights, SessionSet watching_sessions, SI64 man, SI32 exp ) : Model( rights, watching_sessions ), man( man ), exp( exp ) {
}

Val::Val( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    man = inp->read();
    exp = inp->read();
}

Val::operator SI64() const {
    return man;
}

void Val::write_str( Stream &out ) const {
    out << man;
    if ( exp )
        out << "e" << exp;
}

void Val::write_dmp( BinOut &out ) const {
    out << man << exp;
}

bool Val::write_ujs( Stream &nut, Stream &uut, Session *s ) const {
    if ( exp ) {
        if ( exp == std::numeric_limits<SI32>::max() ) // NaN
            uut << "FileSystem._objects[" << this << "].set(NaN);\n";
        else
            uut << "FileSystem._objects[" << this << "].set(" << man << "*Math.pow(10," << exp << "));\n";
    } else
        uut << "FileSystem._objects[" << this << "].set(" << man << ");\n";
    return true;
}

bool Val::write_usr( BinOut &nut, BinOut &uut, Session *s ) const {
    uut << 'W' << PI64( this ) << man << exp;
    return true;
}

Nstring Val::type() const {
    return NSTRING_Val;
}

int Val::type_dump() const {
    return _Val;
}

bool Val::_set( const TmpModelMap &mm, StringBlk data ) {
    SI64 oman = man;
    SI32 oexp = exp;

    if ( data == "NaN" or data == "nan" or data == "Nan" ) {
        man = 0;
        exp = std::numeric_limits<SI32>::max();
    } else {
        // -
        int i = 0;
        bool minus = false;
        if ( data[ i ] == '-' ) {
            minus = true;
            ++i;
        }

        // man
        man = 0;
        for( ; i < data.size(); ++i ) {
            if ( data[ i ] < '0' or data[ i ] > '9' )
                break;
            man = 10 * man + ( data[ i ] - '0' );
        }
        if ( minus )
            man = -man;

        // .
        exp = 0;
        if ( data[ i ] == '.' ) {
            for( ++i; i < data.size(); ++i ) {
                if ( data[ i ] < '0' or data[ i ] > '9' )
                    break;
                man = 10 * man + ( data[ i ] - '0' );
                --exp;
            }
        }

        // exp
        if ( data[ i ] == 'e' ) {
            bool minus = false;
            if ( data[ ++i ] == '-' ) {
                minus = true;
                ++i;
            }
            for( ; i < data.size(); ++i ) {
                if ( data[ i ] < '0' or data[ i ] > '9' )
                    break;
                exp = 10 * exp + ( data[ i ] - '0' );
            }
            if ( minus )
                exp = -exp;
        }
    }

    return oman != man or oexp != exp;
}

void Val::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
}

