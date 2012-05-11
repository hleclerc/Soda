#ifndef MODEL_H
#define MODEL_H

#include "../Sys/TypeConfig.h"
#include "../Sys/StringBlk.h"
#include "../Sys/Nstring.h"
#include "../Sys/Stream.h"
#include "../Sys/Vec.h"

#include "../Database/SessionSet.h"
#include "../Database/RightSet.h"

#include <map>

class TmpModelMap;
class Session;
class MapRead;
class BinOut;
class BinInp;

/**
  modifications are made with MP
*/
class Model {
public:
    // used for binary files -> please do not change the order (to maintain the binary compatibility)
    enum {
        _DbFileVersion,
        _RootDir,
        _RootUsr,
        _Nstring,
        _Rights,

        #define DECL( M ) _##M,
        DECL( ModelWithAttrAndName )
        #include "Model_decl.h"
        #undef DECL
    };

    Model( RightSet rights, SessionSet watching_sessions );
    virtual ~Model();

    // reading
    virtual int nb_attr() const;
    virtual Model *attr( int index ) const; ///< attr number $index
    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;

    virtual operator SI64() const;
    virtual operator String() const;

    virtual bool equal( StringBlk name ) const;

    virtual void map_ptr( const MapRead &map_read ) = 0; ///< fake ptrs (from file) to real ptrs
    virtual int type_dump() const = 0;
    virtual Nstring type() const = 0;

    virtual void write_str( Stream &out ) const = 0;
    virtual void write_dmp( BinOut &out ) const = 0;
    virtual void write_ujs( Stream &out, Session *s ) const = 0; ///< update object in javascript
    bool write_njs( Stream &out, int var, Session *s ) const; ///< code for new v_$var javascript variable representing this

    void rm_inactive_sessions(); ///< rm from watching_sessions if s->inactive

    const Database *db() const;
    Database *db();

    mutable SessionSet watching_sessions; ///<
    Session           *mod_session; ///< session that made the last change (0 means no change)
    Model             *next_mod; ///< next model in mod_list (mod_session != 0 means that *this is inside a mod_list)
    RightSet           rights;

    static PI64        cur_op_id; ///<
    PI64               op_id; ///<

    virtual bool _write_njs( Stream &out, int var, Session *s ) const = 0; ///< returns true if ok
    virtual bool _set( const TmpModelMap &mm, StringBlk data ) = 0;
};

#endif // MODEL_H
