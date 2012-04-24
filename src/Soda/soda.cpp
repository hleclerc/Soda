#define PREPARG_GET_SET_PREFIX loop.
#include "Com/SodaLoop.h"
#include <stdio.h>

//
#include <PrepArg/usage.h>

int main( int argc, char **argv ) {
    SodaLoop loop;
    #include <PrepArg/parse.h>
    return loop.run();
}

