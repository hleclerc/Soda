#ifndef NSTRINGLIST_H
#define NSTRINGLIST_H

#include "StringBlk.h"
#include "Nstring.h"
class BinInp;

/**
  Basically an allocator for (more or less) permanent strings, stored in Nstring::Item
*/
class NstringList {
public:
    static const int size_hash_list = 1024;
    typedef Nstring::Item Item;

    NstringList();

    Nstring operator()( BinInp &inp );
    Nstring operator()( StringBlk s );
    Nstring operator()( const char *beg );
    Nstring operator()( const char *beg, int len );
    Nstring operator()( const char *beg, int len, unsigned hash_val );

    int size() const; ///< nb nstrings

    Item *lmod;
protected:
    Item *hash_list[ size_hash_list ];
    int nb_nstrings;
};

#endif // NSTRINGLIST_H
