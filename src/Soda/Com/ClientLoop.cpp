#include "ClientLoop.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>

static int connected_socket( const char *host, const char *port ) {
    // stolen from https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
    addrinfo hints;
    memset( &hints, 0, sizeof( addrinfo ) );
    hints.ai_family   = AF_UNSPEC;   /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags    = AI_PASSIVE;  /* All interfaces */

    addrinfo *result;
    if ( int s = getaddrinfo( NULL, port, &hints, &result ) ) {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( s ) );
        return -1;
    }

    for ( addrinfo *rp = result; rp; rp = rp->ai_next ) {
        // socket ok ?
        int listen_fd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        if ( listen_fd < 0 )
            continue;

        // bind ok ?
        if ( connect( listen_fd, rp->ai_addr, rp->ai_addrlen ) == 0 ) {
            freeaddrinfo( result );
            return listen_fd;
        }

        close( listen_fd );
    }

    fprintf( stderr, "Could not connect\n" );
    freeaddrinfo( result );
    return -1;
}

ClientLoop::ClientLoop() {
    port = "8890";
    host = "localhost";
}

int ClientLoop::run() {
    fd = connected_socket( host, port );

    const char msg[] = "pouet";
    write( fd, msg, sizeof( msg ) );
    return 0;
}
