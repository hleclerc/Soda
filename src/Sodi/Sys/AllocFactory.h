#ifndef ALLOCFACTORY_H
#define ALLOCFACTORY_H

#include "TypeConfig.h"
#include "N.h"
#include "S.h"
#include <new>

/**
    Permits to make new object without repeating the type
        int *a = my_alloc.factory(); -> will produce an int
*/
template<class Allocator>
struct AllocFactory : public Allocator {
    /// @see PermAlloc, ...Alloc, ...
    struct Factory_0 {
        Factory_0( Allocator *l ) : l( l ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI;
        }
        Allocator *l;
    };

    /// @see PermAlloc, ...Alloc, ...
    template<class T0>
    struct Factory_1 {
        Factory_1( Allocator *l, const T0 &a0 ) : l( l ), a0( a0 ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI( a0 );
        }
        Allocator *l;
        const T0 &a0;
    };

    /// @see PermAlloc, ...Alloc, ...
    template<class T0,class T1>
    struct Factory_2 {
        Factory_2( Allocator *l, const T0 &a0, const T1 &a1 ) : l( l ), a0( a0 ), a1( a1 ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI( a0, a1 );
        }
        Allocator *l;
        const T0 &a0;
        const T1 &a1;
    };

    /// @see PermAlloc, ...Alloc, ...
    template<class T0,class T1,class T2>
    struct Factory_3 {
        Factory_3( Allocator *l, const T0 &a0, const T1 &a1, const T2 &a2 ) : l( l ), a0( a0 ), a1( a1 ), a2( a2 ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI( a0, a1, a2 );
        }
        Allocator *l;
        const T0 &a0;
        const T1 &a1;
        const T2 &a2;
    };

    /// @see PermAlloc, ...Alloc, ...
    template<class T0,class T1,class T2,class T3>
    struct Factory_4 {
        Factory_4( Allocator *l, const T0 &a0, const T1 &a1, const T2 &a2, const T3 &a3 ) : l( l ), a0( a0 ), a1( a1 ), a2( a2 ), a3( a3 ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI( a0, a1, a2, a3 );
        }
        Allocator *l;
        const T0 &a0;
        const T1 &a1;
        const T2 &a2;
        const T3 &a3;
    };

    /// @see PermAlloc, ...Alloc, ...
    template<class T0,class T1,class T2,class T3,class T4>
    struct Factory_5 {
        Factory_5( Allocator *l, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4 ) : l( l ), a0( a0 ), a1( a1 ), a2( a2 ), a3( a3 ), a4( a4 ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            return new( l->allocate( N<sizeof(TI)>() ) ) TI( a0, a1, a2, a3, a4 );
        }
        Allocator *l;
        T0 a0;
        T1 a1;
        T2 a2;
        T3 a3;
        T4 a4;
    };

    /// @see PermAlloc, ...Alloc, ...
    struct Factory_V {
        Factory_V( Allocator *l, ST size ) : l( l ), size( size ) {}
        template<class TI> operator TI *() {
            if ( sizeof( TI ) >= 4 )
                l->align( N<4>() );
            TI *res = reinterpret_cast<TI *>( l->allocate( sizeof( TI ) * size ) );
            for( ST i = 0; i < size; ++i )
                new ( res + i ) TI;
            return res;
        }
        Allocator *l;
        ST size;
    };

    /// ex : int *a = local_mem_alloc.factory(); -> will produce an int
    Factory_0 factory() {
        return this;
    }

    /// ex : int *a = local_mem_alloc.factory( 10.5 ); -> will produce an int with value 10
    template<class T0>
    Factory_1<T0> factory( const T0 &a0 ) {
        return Factory_1<T0>( this, a0 );
    }

    /// ex : int *a = local_mem_alloc.factory( 10.5 ); -> will produce an int with value 10
    template<class T0,class T1>
    Factory_2<T0,T1> factory( const T0 &a0, const T1 &a1 ) {
        return Factory_2<T0,T1>( this, a0, a1 );
    }

    /// ex : int *a = local_mem_alloc.factory( 10.5 ); -> will produce an int with value 10
    template<class T0,class T1,class T2>
    Factory_3<T0,T1,T2> factory( const T0 &a0, const T1 &a1, const T2 &a2 ) {
        return Factory_3<T0,T1,T2>( this, a0, a1, a2 );
    }

    /// ex : int *a = local_mem_alloc.factory( 10.5 ); -> will produce an int with value 10
    template<class T0,class T1,class T2,class T3>
    Factory_4<T0,T1,T2,T3> factory( const T0 &a0, const T1 &a1, const T2 &a2, const T3 &a3 ) {
        return Factory_4<T0,T1,T2,T3>( this, a0, a1, a2, a3 );
    }

    /// ex : int *a = local_mem_alloc.factory( 10.5 ); -> will produce an int with value 10
    template<class T0,class T1,class T2,class T3,class T4>
    Factory_5<T0,T1,T2,T3,T4> factory( T0 a0, T1 a1, T2 a2, T3 a3, T4 a4 ) {
        return Factory_5<T0,T1,T2,T3,T4>( this, a0, a1, a2, a3, a4 );
    }

    Factory_V factory_vec( ST size ) {
        return Factory_V( this, size );
    }

    template<class TI> TI *New() {
        return new( allocate( N<sizeof( TI )>() ) ) TI;
    }

};

#endif // ALLOCFACTORY_H
