#include <Soda/Sys/Stream.h>
#include <Soda/Sys/IdSet.h>

int main() {
    IdSetList<int,1,double> l;
    IdSet<int,1,double> n = l.no_Id;

    PRINT( n << 10 );
    PRINT( n << 100 );
    PRINT( n >> 100 );
}
