#ifndef MP_H
#define MP_H

#include "../Sys/StringBlk.h"
#include "../Sys/Nstring.h"
#include "../Sys/Stream.h"
#include "Model.h"
class Session;

/**

*/
class MP {
public:
    MP( Session *s = 0 );
    MP( Session *s, Model *m ) : s( s ), m( m ) {}
    MP( Session *s, Model *m, Nstring n ) : s( s ), m( m ), n( n ) {}
    MP( Session *s, Model *m, StringBlk b ) : s( s ), m( m ), b( b ) {}

    // accessors
    MP operator[]( StringBlk path ); ///<
    MP operator[]( Nstring path ); ///<

    // readers
    void write_str( Stream &out ) const;

    // helpers struct for the factories
    struct New_File { StringBlk name; Model *p; };
    struct New_OS { StringBlk name; };
    struct New_ON { Nstring   name; };
    struct New_Directory {};
    struct New_Lst {};


    // modifiers
    template<class T>
    MP operator=( const T &val ) {
        // we need to create a new attribute ?
        if ( b or n )
            return _create_attr( factory( val ) );
        // else, call modifier
        if ( m and m->rights.has( _user(), RightSet::WR ) and m->_set( val ) )
            _add_to_mod_list( m );
        return *this;
    }

    template<class T>
    MP operator<<( const T &val ) {
        // we need to create a new Lst ?
        if ( b or n )
            return _create_attr( factory( New_Lst() ) ) << val;
        // else, call modifier (assuming m is a kind of list)
        if ( m and m->rights.has( _user(), RightSet::WR ) and m->_push( factory( val ) ) )
            _add_to_mod_list( m );
        return *this;
    }

    // factories
    Model *factory( const std::string &val );
    Model *factory( Model *m ) { return m; }
    Model *factory( New_Directory );
    Model *factory( StringBlk val );
    Model *factory( New_File nf );
    Model *factory( New_OS nf );
    Model *factory( New_ON nf );
    Model *factory( SI64 val );
    Model *factory( New_Lst );

protected:
    void  _add_to_mod_list( Model *m ); ///<
    MP    _create_attr( Model *p ); ///< if b or n
    User *_user(); ///< helper to get user without including Session.h

    Session  *s;
    Model    *m;
    StringBlk b;
    Nstring   n;
};

inline MP::New_OS new_Obj( StringBlk name ) { MP::New_OS res; res.name = name; return res; }
inline MP::New_ON new_Obj( Nstring   name ) { MP::New_ON res; res.name = name; return res; }

inline MP::New_Lst new_Lst() { return MP::New_Lst(); }

inline MP::New_File new_File( StringBlk name, Model *p ) { MP::New_File res; res.name = name; res.p = p; return res; }
inline MP::New_Directory new_Directory() { return MP::New_Directory(); }

#endif // MP_H
