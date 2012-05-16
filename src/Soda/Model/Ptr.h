#ifndef PTR_H
#define PTR_H

#include "Model.h"

/**
*/
class Ptr : public Model {
public:
    Ptr( RightSet rights, SessionSet watching_sessions, PI64 man = 0 );
    Ptr( BinInp *inp, RightSet rights, SessionSet watching_sessions );

    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const;
    virtual operator SI64() const;
    virtual int type_dump() const;
    virtual Nstring type() const;

    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );

    Model *get_model() { return (Model *)man; }
    void set_model( Model *m ) { man = (SI64)m; }

    PI64 man;
};

#endif // PTR_H
