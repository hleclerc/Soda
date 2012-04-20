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
        Item( IdSetList *orig = 0, std::vector<T> *data = 0 ) : orig( orig ), pmod( 0 ) {
            if ( data )
                for( int i = 0; i < n; ++i )
                    this->data[ i ] = data[ i ];
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
    1}

    const Item *lmod; ///< last created Item
    const Item *no_Id; ///< item with nothing (no Id)
    ExtraAttr extra;

protected:
    struct CmpItem { ///< used to order items
        bool operator()( const Item &a, const Item &b ) const {
            for( int j = 0; j < n; ++j ) {
                if ( a.data[ j ].size() < b.data[ j ].size() ) return false;
                if ( a.data[ j ].size() > b.data[ j ].size() ) return true;
                for( int i = 0; i < a.data[ j ].size(); ++i ) {
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
