#ifndef LST_H
#define LST_H

#include "Model.h"

/**
*/
class Lst : public Model {
public:
    Lst( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    Lst( RightSet rights, SessionSet watching_sessions );

    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const;

    virtual int type_dump() const { return _Lst; }
    virtual Nstring type() const;

    virtual int nb_attr() const;
    virtual Model *attr( int index ) const;

    virtual bool _set( int size, Vec<Model *> &model_stack, Vec<String> &string_stack );
    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );
    virtual void _sweeper_rec( Sweeper &s );

    Vec<Model *> _data;
};

#endif // LST_H
