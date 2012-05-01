#ifndef _SECUREALLOC_H
#define _SECUREALLOC_H

#include <stdlib.h>
#include <set>

/**
  TODO: actual check
*/
class _SecureAlloc {
public:
    _SecureAlloc() {
    }

    void *allocate( int size ) {
        void *res = malloc( size );
        _allowed.insert( res );
        return res;
    }

    template<int size>
    inline void *allocate( N<size> ) {
        return allocate( size );
    }

    template<int size>
    void align( N<size> ) {}

    void free( void *ptr, int ) {
        _allowed.erase( ptr );
        ::free( ptr );
    }

    template<class T>
    T *check( T *ptr ) const {
        return  _allowed.count( ptr ) ? ptr : 0;
    }

    std::set<void *> _allowed; // TODO: take an optimized container
};

#endif // _SECUREALLOC_H
