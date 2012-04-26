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
    virtual int type_dump() const { return _Val; }
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual void write_ujs( Stream &out, Session *s ) const;
    virtual Nstring type() const;

    virtual operator SI64() const;
    virtual operator FP64() const;

    SI64 man;
    SI32 exp;

protected:
    virtual void _write_njs( Stream &out, int var, Session *s ) const;
    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual bool _set( StringBlk data );
    virtual bool _set( SI64 val );
    friend class MP;
};

#endif // VAL_H
