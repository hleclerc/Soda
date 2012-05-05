#ifndef VAL_H
#define VAL_H

#include "Model.h"

/**
*/
class Val : public Model {
public:
    Val( RightSet rights, SessionSet watching_sessions, SI64 man = 0, SI32 exp = 0 );
    Val( BinInp *inp, RightSet rights, SessionSet watching_sessions );

    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual void write_ujs( Stream &out, Session *s ) const;
    virtual operator SI64() const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    virtual void _write_njs( Stream &out, int var, Session *s ) const;
    virtual bool _set( const TmpModelMap &mm, StringBlk data );

    SI64 man;
    SI32 exp;
};

#endif // VAL_H
