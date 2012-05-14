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

void Str::write_ujs( Stream &out, Session * ) const {
    out << "FileSystem._objects[ " << this << " ].set( decodeURIComponent( '" << url_encode( _data ) << "' ), false );\n";
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

bool Str::_write_njs( Stream &out, int var, Session *s ) const {
    out << "var v_" << var << " = new Str( decodeURIComponent( '" << url_encode( _data ) << "' ) );\n";
    return true;
}

bool Str::_set( const TmpModelMap &mm, StringBlk data ) {
    std::string tmp = url_decode( std::string( data.c_str(), data.c_str() + data.size() ) );
    bool res = not ( tmp == _data );
    _data = tmp;
    return res;
}

void Str::_map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
}

