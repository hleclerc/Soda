#ifndef MODEL_H
#define MODEL_H

#include "../Sys/TypeConfig.h"
#include "../Sys/StringBlk.h"
#include "../Sys/Nstring.h"
#include "../Sys/Stream.h"
#include "../Sys/Vec.h"
#include "SessionSet.h"
#include "RightSet.h"
#include <map>
class Session;
class MapRead;
class BinOut;
class BinInp;

/**
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
        #include "Model_decl.h"
        #undef DECL
    };

    Model( RightSet rights, SessionSet watching_sessions );
    virtual ~Model();

    virtual operator SI64() const;
    virtual operator FP64() const;
    virtual operator std::string() const;

    virtual int nb_attr() const;
    virtual Model *attr( int index ) const;
    virtual Model *attr( Nstring name ) const;
    virtual Model *attr( StringBlk name ) const;

    virtual bool equal( StringBlk name ) const;

    virtual void map_ptr( const MapRead &map_read ) = 0; ///< fake ptrs (from file) to real ptrs
    virtual int type_dump() const = 0;
    virtual Nstring type() const = 0;

    virtual void write_str( Stream &out ) const = 0;
    virtual void write_dmp( BinOut &out ) const = 0;

    const Database *db() const;
    Database *db();

    SessionSet   watching_sessions; ///<
    Session     *mod_session; ///< session that made the last change (0 means no change)
    Model       *next_mod; ///<
    Vec<Model *> parents;
    RightSet     rights;

    PI64         op_id; ///< nb rounds * 2
    static PI64  cur_op_id; ///< nb rounds * 2

protected:
    friend class MP;

    template<class T>
    bool _set( const T & ) { return false; }

    virtual bool _set( StringBlk data ) = 0;
    virtual bool _set( Model *m );
    virtual bool _set( SI32 val ) { return _set( SI64( val ) ); }
    virtual bool _set( SI64 val );
    virtual bool _set( FP64 val );

    virtual bool _add_attr( Session *s, StringBlk name, Model *m );
    virtual bool _add_attr( Session *s, Nstring name, Model *m );

    virtual bool _push( Model *m );
};

#endif // MODEL_H
