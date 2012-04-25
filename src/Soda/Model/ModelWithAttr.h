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

    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual void write_ujs( Stream &out, Session *s ) const;

protected:
    virtual bool _set( StringBlk data );

    virtual bool _add_attr( Session *s, StringBlk name, Model *m );
    virtual bool _add_attr( Session *s, Nstring name, Model *m );

    virtual void _write_njs( Stream &out, int var, Session *s ) const;

    void _map_ptr( const MapRead &map_read );

    friend class MP;
    Vec<Item> _data;
};

#endif // MODELWITHATTR_H
