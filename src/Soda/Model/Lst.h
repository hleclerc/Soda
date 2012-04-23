#ifndef LST_H
#define LST_H

#include "Model.h"

/**
*/
class Lst : public Model {
public:
    Lst( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    Lst( RightSet rights, SessionSet watching_sessions );
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;

    virtual int type_dump() const { return _Lst; }
    virtual Nstring type() const;

    virtual int nb_attr() const;
    virtual Model *attr( int index ) const;

protected:
    virtual void _write_njs( Stream &out, int var, Session *s ) const;
    virtual bool _set( StringBlk data );
    virtual bool _push( Model *m );

    friend class MP;
    Vec<Model *> _data;
};

#endif // LST_H
