#ifndef MODELWITHATTR_H
#define MODELWITHATTR_H

#include "Model.h"

/**
*/
class ModelWithAttr : public Model {
public:
    struct Item {
        Nstring key;
        Model  *val;
    };

    ModelWithAttr( RightSet rights, SessionSet watching_sessions );

    virtual int nb_attr() const;
    virtual Model *attr( int index ) const;
    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;

    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;

protected:
    virtual bool _set( StringBlk data );

    virtual bool _add_attr( Session *s, StringBlk name, Model *m );
    virtual bool _add_attr( Session *s, Nstring name, Model *m );

    Vec<Item> _data;
};

#endif // MODELWITHATTR_H
