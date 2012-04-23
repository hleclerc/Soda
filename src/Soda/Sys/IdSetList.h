#ifndef IDSETLIST_H
#define IDSETLIST_H

#include <vector>
#include <set>

/**
  List of IdSet<T,n>

  T must be something which support comparision (e.g. int to store int ids)
  n is the number of sub lists in each IdSet
*/
template< class T, int n = 1, class ExtraAttr = int >
class IdSetList {
public:
    struct Item {
        enum { _n = n };

        Item( IdSetList *orig = 0, std::vector<T> *data = 0 ) : orig( orig ), pmod( 0 ) {
            if ( data )
                for( int i = 0; i < n; ++i )
                    this->data[ i ] = data[ i ];
        }

        template<class BI>
        Item( BI &inp, IdSetList *orig ) : orig( orig ), pmod( 0 ) {
            for( int i = 0; i < n; ++i ) {
                int s = inp.read();
                data[ i ].resize( s );
                for( int j = 0; j < s; ++j )
                    data[ i ][ j ] = inp.read();
            }
        }

        template<class BO>
        void write_dmp( BO &out ) const {
            for( int i = 0; i < n; ++i ) {
                out << (int)data[ i ].size();
                for( unsigned j = 0; j < data[ i ].size(); ++j )
                    out << data[ i ][ j ];
            }
        }

        IdSetList *orig;
        std::vector<T> data[ n ];
        mutable const Item *pmod;
    };

    IdSetList() : lmod( 0 ) {
        no_Id = find( 0 );
    }

    const Item *find( std::vector<T> *data ) {
        std::pair<typename std::set<Item,CmpItem>::iterator, bool> iter = sets.insert( Item( this, data ) );
        const Item *res = &( *iter.first );
        // new element ?
        if ( iter.second ) {
            res->pmod = lmod;
            lmod = res;
        }
        return res;
    }

    const Item *lmod; ///< last created Item
    const Item *no_Id; ///< item with nothing (no Id)
    ExtraAttr extra;

protected:
    struct CmpItem { ///< used to order items
        bool operator()( const Item &a, const Item &b ) const {
            for( int j = 0; j < n; ++j ) {
                if ( a.data[ j ].size() < b.data[ j ].size() ) return false;
                if ( a.data[ j ].size() > b.data[ j ].size() ) return true;
                for( unsigned i = 0; i < a.data[ j ].size(); ++i ) {
                    const T &va = a.data[ j ][ i ];
                    const T &vb = b.data[ j ][ i ];
                    if ( va > vb ) return false;
                    if ( va < vb ) return true;
                }
            }
            return false;
        }
    };

    std::set<Item,CmpItem> sets;
};

#endif // IDSETLIST_H
