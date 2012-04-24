#include <Soda/Database/RootSession.h>
#include <Soda/Database/Database.h>

void make() {
    unlink( "dump.db" );

    Database db;
    RootSession rs( &db, "dump.db" );

    rs[ "/toto" ] = 10;
    rs[ "/tata" ] = 11;
    rs[ "/home" ] = new_Directory();
    rs[ "/home/tata" ] = 12;

    PRINT( rs[ "/" ] );
    PRINT( rs[ "/home" ] );
    PRINT( rs[ "/home/tata" ] );

    db.end_round();
}

void read() {
    Database db;
    RootSession rs( &db, "dump.db" );

    PRINT( rs[ "/" ] );
    PRINT( rs[ "/toto" ] );
    PRINT( rs[ "/tata" ] );
    PRINT( rs[ "/home" ] );
    PRINT( rs[ "/home/tata" ] );
}

int main() {
    make();
    read();
}
