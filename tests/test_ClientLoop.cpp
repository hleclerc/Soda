#include <Soda/Com/ClientLoop.h>

int main( int argc, char **argv ) {
    ClientLoop loop;
    loop.port = "8890";
    return loop.run();
}


