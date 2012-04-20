#ifndef NSTRINGHASH_H
#define NSTRINGHASH_H

inline unsigned nstring_hash( const char *beg, int len ) {
    static const unsigned m[] = { 83,263,151,199,619,311,347,19,601,43,239,523,463,599,223,29 };

    unsigned res = 0;
    for( int i = 0; i < len; ++i )
        res += beg[ i ] * m[ i % 16 ];
    return res;
}

#endif // NSTRINGHASH_H
