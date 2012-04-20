#include <Sodi/Sys/NstringList.h>
#include <Sodi/Sys/Stream.h>

int main() {
    NstringList nl;
    PRINT( nl( "pouet" ) );
    PRINT( nl.size() );
    PRINT( nl( "pouet" ) );
    PRINT( nl.size() );
}
