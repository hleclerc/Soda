#ifndef STR_H
#define STR_H

#include "Model.h"

/**

*/
class Str : public Model {
public:
    Str( RightSet rights, SessionSet watching_sessions, const std::string &_data = "" );
    Str( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    virtual operator String() const;
    virtual int type_dump() const { return _Str; }
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual bool equal( StringBlk name ) const;
    virtual Nstring type() const;

    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );

    std::string _data;
};

#endif // STR_H
