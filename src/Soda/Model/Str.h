#ifndef STR_H
#define STR_H

#include "Model.h"

/**

*/
class Str : public Model {
public:
    Str( RightSet rights, SessionSet watching_sessions, const std::string &_data = "" );
    Str( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    virtual int type_dump() const { return _Str; }
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual void write_ujs( Stream &out, Session *s ) const;
    virtual bool equal( StringBlk name ) const;
    virtual Nstring type() const;

    virtual operator std::string() const;

protected:
    virtual void _write_njs( Stream &out, int var, Session *s ) const;
    virtual bool _set( StringBlk data );

    friend class MP;
    std::string _data;
};

#endif // STR_H
