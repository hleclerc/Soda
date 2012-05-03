#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinInp.h"
#include "../Sys/BinOut.h"
#include "../Sys/Math.h"
#include "Str.h"

#include <Celo/StringHelp.h>

Str::Str( RightSet rights, SessionSet watching_sessions, const std::string &_data ) : Model( rights, watching_sessions ), _data( _data) {
}

Str::Str( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : Model( rights, watching_sessions ) {
    int size = inp->read();
    char *_ata = new char[ ceil( size, 4 ) ];
    inp->read( _ata, ceil( size, 4 ) );
    _data.assign( _ata, _ata + size );
    delete [] _ata;
}

void Str::map_ptr( const MapRead &map_read ) {
    rights = map_read[ rights ];
}

void Str::write_str( Stream &out ) const {
    out << _data;
}

void Str::write_ujs( Stream &out, Session * ) const {
    out << "FileSystem._objects[ " << this << " ].set( decodeURIComponent( '" << url_encode( _data ) << "' ), false );\n";
}

void Str::write_dmp( BinOut &out ) const {
    out << (int)_data.size();
    out.write( _data.data(), _data.size() );
    if ( int e = ceil( _data.size(), 4 ) - _data.size() ) {
        const char *r = "    ";
        out.write( r, e );
    }
}

bool Str::equal( StringBlk data ) const {
    return StringBlk( _data.data(), _data.size() ) == data;
}

Nstring Str::type() const {
    return NSTRING_Str;
}

Str::operator std::string() const {
    return _data;
}

void Str::_write_njs( Stream &out, int var, Session *s ) const {
    out << "var v_" << var << " = new Str( decodeURIComponent( '" << url_encode( _data ) << "' ) );\n";
}

bool Str::_set( const TmpModelMap &mm, StringBlk data ) {
    return _set( data );
}

bool Str::_set( StringBlk data ) {
    std::string tmp = url_decode( std::string( data.c_str(), data.c_str() + data.size() ) );
    bool res = not ( tmp == _data );
    _data = tmp;
    return res;
}
