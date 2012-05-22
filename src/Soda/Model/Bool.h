#ifndef BOOL_H
#define BOOL_H

#include "Model.h"

/**
*/
class Bool : public Model {
public:
    Bool( RightSet rights, SessionSet watching_sessions, bool val = false );
    Bool( BinInp *inp, RightSet rights, SessionSet watching_sessions );

    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const;
    virtual operator SI64() const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );

    unsigned char _data;
};

#endif // BOOL_H
