#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include "Vec.h"

/**
  Wrapper for the system call (for external commands)
*/
class Command {
public:
    static void feed_stream_with( std::ostream &os, const std::string &inp );
    int exec( const std::string &base, const std::string &c, std::string dst = "", Vec<std::string> src = Vec<std::string>() );

    int return_code;
    std::string cmd;   ///<
    std::string out;   ///<
    std::string err;   ///<
};

std::ostream &operator<<( std::ostream &os, const Command &cmd );

#endif // COMMAND_H
