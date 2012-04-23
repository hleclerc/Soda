#include <Soda/Sys/NstringList.h>
#include <Soda/Sys/Stream.h>

int main() {
    NstringList nl;
    PRINT( nl( "pouet" ) );
    PRINT( nl.size() );
    PRINT( nl( "pouet" ) );
    PRINT( nl.size() );
}
