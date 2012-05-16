#ifndef VAL_H
#define VAL_H

#include "Model.h"

/**
*/
class Val : public Model {
public:
    Val( RightSet rights, SessionSet watching_sessions, SI64 man = 0, SI32 exp = 0 );
    Val( BinInp *inp, RightSet rights, SessionSet watching_sessions );

    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual operator SI64() const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );

    SI64 man;
    SI32 exp;
};

#endif // VAL_H
