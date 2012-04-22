#ifndef LST_H
#define LST_H

#include "Model.h"

/**
*/
class Lst : public Model {
public:
    Lst( RightSet rights, SessionSet watching_sessions );
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual Nstring type() const;

protected:
    virtual bool _set( StringBlk data );
    virtual bool _push( Model *m );

    Vec<Model *> _data;
};

#endif // LST_H
