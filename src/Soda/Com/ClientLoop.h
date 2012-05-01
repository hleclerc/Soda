#ifndef CLIENTLOOP_H
#define CLIENTLOOP_H



/**
  event loop for a cpp cient
*/
class ClientLoop {
public:
    ClientLoop();
    int run();

    // input
    const char *port;
    const char *host;

    //
    int   fd;
};

#endif // CLIENTLOOP_H
