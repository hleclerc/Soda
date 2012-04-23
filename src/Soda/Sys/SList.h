#ifndef SLIST_H
#define SLIST_H

struct SListDefaultGetPrev {
    template<class Item>
    Item *&operator()( Item *item ) const {
        return item->prev;
    }
    template<class Item>
    const Item *operator()( const Item *item ) const {
        return item->prev;
    }
};

/// linked list with last and prev
template<class T,class GetPrev=SListDefaultGetPrev>
struct SList {
    struct SList_Exception {};

    SList() : last( 0 ) {}

    SList &operator<<( T *item ) {
        get_prev( item ) = last;
        last = item;
        return *this;
    }

    SList &operator<<( SList &lst ) {
        if ( T *beg = lst.last ) {
            while ( T *prev = get_prev( beg ) )
                beg = prev;
            //
            get_prev( beg ) = last;
            last = lst.last;
        }
        return *this;
    }

    T *pop_back() {
        T *res = last;
        if ( res )
            last = get_prev( res );
        return res;
    }

    const T &operator[]( ST n ) const {
        n = size() - 1 - n;
        for( const T *i = last; i; i = get_prev( i ), --n )
            if( n == 0 )
                return *i;
        throw SList_Exception();
    }

    T &operator[]( ST n ) {
        n = size() - 1 - n;
        for( T *i = last; i; i = get_prev( i ), --n )
            if( n == 0 )
                return *i;
        throw SList_Exception();
    }

    ST size() const {
        ST res = 0;
        for( T *i = last; i; i = get_prev( i ) )
            ++res;
        return res;
    }

    operator bool() const { return last; }

    GetPrev get_prev;
    T *last;
};


#endif // SLIST_H
