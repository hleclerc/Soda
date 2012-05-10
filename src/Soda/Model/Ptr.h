#ifndef PTR_H
#define PTR_H

#include "Model.h"

/**
*/
class Ptr : public Model {
public:
    Ptr( RightSet rights, SessionSet watching_sessions, SI64 man = 0 );
    Ptr( BinInp *inp, RightSet rights, SessionSet watching_sessions );

    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual void write_ujs( Stream &out, Session *s ) const;
    virtual operator SI64() const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    virtual bool _write_njs( Stream &out, int var, Session *s ) const;
    virtual bool _set( const TmpModelMap &mm, StringBlk data );

    Model *get_model() { return (Model *)man; }
    void set_model( Model *m ) { man = (SI64)m; }

    SI64 man;
};

#endif // PTR_H
