#ifndef IDSET_H
#define IDSET_H

#include "IdSetList.h"
#include <algorithm>

/**
  Pointer on n set of data of type T.

  T must be something which support comparision (an int, a pointer, ...)
  n is the number of sub lists
  ExtraAttr is the type of item->orig->extra (accessible by the operator-> )
*/
template< class T, int n = 1, class ExtraAttr = int >
class IdSet {
public:
    typedef typename IdSetList< T, n, ExtraAttr >::Item Item;

    IdSet( const Item *_ptr = 0 ) : _ptr( _ptr ) {}

    template<class OS> void write_str( OS &os ) const {
        for( int i = 0; i < n; ++i )
            for( int j = 0; j < _ptr->data[ i ].size(); ++j )
                os << ( j ? " " : ( i ? " ; " : "" ) ) << _ptr->data[ i ][ j ];
    }

    IdSet &operator<<( T id ) { return add( id, 0 ); } ///< add id
    IdSet &operator>>( T id ) { return rem( id, 0 ); } ///< rem id

    ExtraAttr &operator*() { return _ptr->orig->extra; }
    ExtraAttr &operator->() { return _ptr->orig->extra; }

    IdSet &add( T id, int ns ) {
        // if id already in data
        for( int i = 0; i < _ptr->data[ ns ].size(); ++i )
            if ( _ptr->data[ ns ][ i ] == id )
                return *this;

        // else, make a new list set. TODO: optimize
        std::vector<T> data[ n ];
        for( int i = 0; i < n; ++i )
            data[ i ] = _ptr->data[ i ];
        data[ ns ].push_back( id );
        std::sort( data[ ns ].begin(), data[ ns ].end() );

        //
        _ptr = _ptr->orig->find( data );
        return *this;
    }

    IdSet &rem( T id, int ns ) {
        // if id not in data -> do nothing
        for( int i = 0; ; ++i ) {
            if ( i == _ptr->data[ ns ].size() )
                return *this;
            // else, make a new list set
            if ( _ptr->data[ ns ][ i ] == id ) {
                std::vector<T> data[ n ];
                for( int j = 0; j < n; ++j )
                    data[ j ] = _ptr->data[ j ];
                data[ ns ].erase( data[ ns ].begin() + i );

                _ptr = _ptr->orig->find( data );
                return *this;
            }
        }
    }

    bool has( T id, int ns = 0 ) const {
        for( int i = 0; i < _ptr->data[ ns ].size(); ++i )
            if ( _ptr->data[ ns ][ i ] == id )
                return true;
        return false;
    }

    int size( int ns = 0 ) const { return _ptr->data[ ns ].size(); }
    T operator[]( int index ) const { return _ptr->data[ 0 ][ index ]; }

    bool operator<( Item a ) const { return a._ptr < _ptr; }

protected:
    const Item *_ptr;
};


#endif // IDSET_H
