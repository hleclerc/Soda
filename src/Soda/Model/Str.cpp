#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinInp.h"
#include "../Sys/BinOut.h"
#include "Str.h"

#include <Celo/StringHelp.h>

Str::Str( RightSet rights, SessionSet watching_sessions, const std::string &_data ) : Model( rights, watching_sessions ), _data( _data) {
}

Str::Str( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    *inp >> _data;
}

Str::operator String() const {
    return _data;
}

void Str::write_str( Stream &out ) const {
    out << _data;
}

bool Str::write_ujs( Stream &nut, Stream &uut, Session *s ) const {
    uut << "FileSystem._objects[ " << this << " ].set( decodeURIComponent( '" << url_encode( _data ) << "' ), false );\n";
    return true;
}

bool Str::write_usr( BinOut &nut, BinOut &uut, Session *s ) const {
    uut << 'W' << PI64( this ) << _data;
    return true;
}

void Str::write_dmp( BinOut &out ) const {
    out << _data;
}

bool Str::equal( StringBlk data ) const {
    return StringBlk( _data.data(), _data.size() ) == data;
}

Nstring Str::type() const {
    return NSTRING_Str;
}

bool Str::_set( const TmpModelMap &mm, StringBlk data ) {
    std::string tmp = url_decode( std::string( data.c_str(), data.c_str() + data.size() ) );
    bool res = not ( tmp == _data );
    _data = tmp;
    return res;
}

bool Str::_set( const char *str, int len ) {
    std::string tmp( str, str + len );
    if ( _data == tmp )
        return false;
    _data = tmp;
    return true;
}

void Str::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
}

