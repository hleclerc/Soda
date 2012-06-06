#ifndef MODELWITHATTR_H
#define MODELWITHATTR_H

#include "Model.h"

/**
*/
class ModelWithAttr : public Model {
public:
    struct Item {
        bool operator!=( const Item &attr ) const { return attr.key != key or attr.val != val; }
        Nstring key;
        Model  *val;
    };

    ModelWithAttr( BinInp *inp, RightSet rights, SessionSet watching_sessions );
    ModelWithAttr( RightSet rights, SessionSet watching_sessions );

    virtual int nb_attr() const;
    virtual Model *attr( int index ) const;
    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;

    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const;
    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const;

protected:
    virtual bool _set( const TmpModelMap &mm, StringBlk data );
    virtual void _map_ptr( const MapRead &map_read );
    virtual void _sweeper_rec( Sweeper &s );

    friend class MP;
    Vec<Item> _data;
};

#endif // MODELWITHATTR_H
