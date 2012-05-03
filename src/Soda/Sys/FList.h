#ifndef FLIST_H
#define FLIST_H

#include "TypeConfig.h"
#include "S.h"

struct FListDefaultGetNext {
    template<class Item> Item *&operator()( Item *item ) const { return item->next; }
    template<class Item> const Item *operator()( const Item *item ) const { return item->next; }
};

struct FListDefaultGetPrev {
    template<class Item> Item *&operator()( Item *item ) const { return item->prev; }
    template<class Item> const Item *operator()( const Item *item ) const { return item->prev; }
};

struct FListDefaultGetList {
    template<class Item,class TR> TR *&operator()( Item *item, S<TR> ) const { return item->list; }
    template<class Item,class TR> const TR *operator()( const Item *item, S<TR> ) const { return item->list; }
};

/// doubled intrusive linked list
/// objects can appear only once
template<class T,class GetPrev=FListDefaultGetPrev,class GetNext=FListDefaultGetNext,class GetList=FListDefaultGetList>
struct FList {
    typedef S<FList> STH;

    struct FList_Exception {};

    FList() : beg( 0 ), end( 0 ) {}

    // push back (at the end)
    FList &operator<<( T *item ) {
        if ( not get_list( item, STH() ) ) {
            get_list( item, STH() ) = this;
            get_prev( item ) = end;
            get_next( item ) = 0;

            if ( end )
                get_next( end ) = item;
            else
                beg = item;

            end = item;
        }
        return *this;
    }

    // remove item
    FList &operator>>( T *item ) {
        if ( get_list( item, STH() ) == this ) {
            get_list( item, STH() ) = 0;

            if ( T *p = get_prev( item ) ) {
                get_next( p ) = get_next( item );
                if ( T *n = get_next( item ) )
                    get_prev( n ) = p;
                else
                    end = p;
            } else {
                beg = get_next( item );
                if ( beg )
                    get_prev( beg ) = 0;
                else
                    end = 0;
            }
        }
        return *this;
    }

    T *pop_front() {
        T *res = beg;
        if ( res ) {
            get_list( res, STH() ) = 0;

            beg = get_next( res );
            if ( beg )
                get_prev( beg ) = 0;
            else
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

    GetList get_list;
    GetPrev get_prev;
    GetNext get_next;
    T *beg;
    T *end;
};

#endif // FLIST_H

