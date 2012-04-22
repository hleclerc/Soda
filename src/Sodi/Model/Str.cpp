#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "Str.h"

Str::Str( RightSet rights, SessionSet watching_sessions, const std::string &_data ) : Model( rights, watching_sessions ), _data( _data) {
}

void Str::map_ptr( const MapRead &map_read ) {
    TODO;
}

void Str::write_str( Stream &out ) const {
    out << _data;
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


bool Str::_set( StringBlk data ) {
    bool res = StringBlk( _data.data(), _data.size() ) != data;
    _data.assign( data.c_str(), data.c_str() + data.size() );
    return res;
}
