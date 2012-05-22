#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"
#include "Bool.h"

Bool::Bool( RightSet rights, SessionSet watching_sessions, bool val ) : Model( rights, watching_sessions ), _data( val ) {
}

Bool::Bool( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    _data = inp->read();
}

Bool::operator SI64() const {
    return _data;
}

void Bool::write_str( Stream &out ) const {
    out << _data;
}

void Bool::write_dmp( BinOut &out ) const {
    out << _data;
}

bool Bool::write_ujs( Stream &nut, Stream &uut, Session *s ) const {
    uut << "FileSystem._objects[" << this << "].set(" << bool( _data ) << ");\n";
    return true;
}

bool Bool::write_usr( BinOut &nut, BinOut &uut, Session *s ) const {
    uut << 'Y' << PI64( this ) << _data;
    return true;
}

Nstring Bool::type() const {
    return NSTRING_Bool;
}

int Bool::type_dump() const {
    return _Bool;
}

bool Bool::_set( const TmpModelMap &mm, StringBlk data ) {
    unsigned char _nata = data.size() and ( data[ 0 ] == '1' or data[ 0 ] == 't' or data[ 0 ] == 'T' );
    bool res = _data != _nata;
    _data = _nata;
    return res;
}

void Bool::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
}

