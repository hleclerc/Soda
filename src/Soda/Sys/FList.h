#ifndef FLIST_H
#define FLIST_H

#include "TypeConfig.h"

struct FListDefaultGetNext {
    template<class Item>
    Item *&operator()( Item *item ) const {
        return item->next;
    }
    template<class Item>
    const Item *operator()( const Item *item ) const {
        return item->next;
    }
};

/// linked list with last and prev
template<class T,class GetNext=FListDefaultGetNext>
struct FList {
    struct FList_Exception {};

    FList() : beg( 0 ), end( 0 ) {}

    FList &operator<<( T *item ) {
        get_next( item ) = 0;
        if ( end ) {
            get_next( end ) = item;
            end = item;
        } else {
            beg = item;
            end = item;
        }
        return *this;
    }

    T *pop_front() {
        T *res = beg;
        if ( res ) {
            beg = get_next( res );
            if ( not beg )
                end = 0;
        }
        return res;
    }

    const T &operator[]( ST n ) const {
        for( const T *i = beg; i; i = get_next( i ), --n )
            if( n == 0 )
                return *i;
        throw FList_Exception();
    }

    T &operator[]( ST n ) {
        for( T *i = beg; i; i = get_next( i ), --n )
            if( n == 0 )
                return *i;
        throw FList_Exception();
    }

    ST size() const {
        ST res = 0;
        for( T *i = beg; i; i = get_next( i ) )
            ++res;
        return res;
    }

    operator bool() const { return beg; }

    GetNext get_next;
    T *beg;
    T *end;
};

#endif // FLIST_H
