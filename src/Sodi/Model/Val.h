#ifndef VAL_H
#define VAL_H

#include "Model.h"

/**
*/
class Val : public Model {
public:
    Val( RightSet rights, SessionSet watching_sessions, SI64 man = 0, SI32 exp = 0 );
    Val( RightSet rights, SessionSet watching_sessions, BinInp &inp );
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual Nstring type() const;

    virtual operator SI64() const;
    virtual operator FP64() const;

protected:
    virtual bool _set( StringBlk data );
    virtual bool _set( SI64 val );

    SI64 man;
    SI32 exp;
};

#endif // VAL_H
