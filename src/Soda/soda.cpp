#define PREPARG_GET_SET_PREFIX loop.
#include "Com/ServerLoop.h"
#include <stdio.h>

//
#include <PrepArg/usage.h>

int main( int argc, char **argv ) {
    ServerLoop loop;
    #include <PrepArg/parse.h>
    return loop.run();
}

