#ifndef STR_H
#define STR_H

#include "Model.h"

/**

*/
class Str : public Model {
public:
    Str( RightSet rights, SessionSet watching_sessions, const std::string &_data = "" );
    virtual void map_ptr( const MapRead &map_read );
    virtual void write_str( Stream &out ) const;
    virtual void write_dmp( BinOut &out ) const;
    virtual bool equal( StringBlk name ) const;
    virtual Nstring type() const;

    virtual operator std::string() const;

protected:
    virtual bool _set( StringBlk data );

    std::string _data;
};

#endif // STR_H
