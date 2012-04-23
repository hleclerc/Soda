#ifndef _SECUREALLOC_H
#define _SECUREALLOC_H

#include <stdlib.h>

/**
  TODO: actual check
*/
class _SecureAlloc {
public:
    _SecureAlloc() {
    }

    void *allocate( int size ) {
        return malloc( size );
    }

    template<int size>
    inline void *allocate( N<size> ) {
        return allocate( size );
    }

    template<int size>
    void align( N<size> ) {}

    void free( void *ptr, int ) {
        ::free( ptr );
    }

    template<class T>
    T *check( T *ptr ) const {
        return ptr;
    }
};

#endif // _SECUREALLOC_H
