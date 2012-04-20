#ifndef NSTRING_H
#define NSTRING_H

#include "Math.h"

/**

*/
class Nstring {
public:
    struct Item {
        template<class BO>
        void write_dmp( BO &out ) const {
            out << size;
            out.write( data, size );
            if ( int e = ceil( size, 4 ) - size ) {
                const char *r = "    ";
                out.write( r, e );
            }
        }

        Item *prev; ///< chained list in hash table
        Item *pmod; ///< chained list of "recently" created elements
        char *data; ///< ptr on a \0 ended string
        int   size; ///< length of the string
    };

    Nstring( Item *item ) : item( item ) {}
    Nstring() : item( 0 ) {}

    operator bool() const { return item; }
    operator const char *() const { return item->data; }

    char operator[]( int i ) const { return item->data[ i ]; }
    int size() const { return item->size; }

    bool operator==( const Item *a ) const { return a == item; }
    bool operator==( Nstring a ) const { return a.item == item; }
    bool operator!=( Nstring a ) const { return a.item != item; }
    bool operator< ( Nstring a ) const { return a.item  < item; }

    template<class BO>
    void write_str( BO &out ) const {
        if ( item )
            out.write( item->data, item->size );
    }

protected:
    Item *item;
};

#endif // NSTRING_H
