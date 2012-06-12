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
    struct Sweeper {
        virtual void operator()( Model *m ) = 0;
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

    virtual int type_dump() const = 0;
    virtual Nstring type() const = 0;
    Nstring js_type() const;

    virtual void write_str( Stream &out ) const = 0;
    virtual void write_dmp( BinOut &out ) const = 0;
    virtual bool write_ujs( Stream &nut, Stream &uut, Session *s ) const = 0; ///< update object (call set, push, ...) in javascript
    virtual bool write_usr( BinOut &nut, BinOut &uut, Session *s ) const = 0; ///< update object

    bool write_njs( Stream &nut, Stream &uut, Session *s ) const; ///<
    bool write_nsr( BinOut &nut, BinOut &uut, Session *s ) const; ///< code for new var

    void rm_inactive_sessions(); ///< rm from watching_sessions if s->inactive
    void map_ptr( const MapRead &map_read ); ///< fake ptrs (from file) to real ptrs
    void sweep( Sweeper &s );

    const Database *db() const;
    Database *db();

    mutable SessionSet watching_sessions; ///<
    Session           *mod_session; ///< session that made the last change (0 means no change)
    Model             *next_mod; ///< next model in mod_list (mod_session != 0 means that *this is inside a mod_list)
    RightSet           rights;
    Nstring            _js_type; ///<

    static PI64        cur_op_id; ///<
    PI64               op_id; ///<
    bool               in_mod_list; ///<


    virtual bool _set( const TmpModelMap &mm, StringBlk data ) = 0; ///< return true if real change
    virtual bool _set( int info, Vec<Model *> &model_stack, Vec<String> &string_stack );
    virtual bool _set( const char *str, int len );
    virtual bool _set( SI64 a, SI32 b );
    virtual bool _set( Model *m );
    virtual bool _set( PI64 a );

    virtual void _map_ptr( const MapRead &map_read ) = 0; ///< fake ptrs (from file) to real ptrs
    virtual void _sweeper_rec( Sweeper &s ); ///<


};

#endif // MODEL_H
