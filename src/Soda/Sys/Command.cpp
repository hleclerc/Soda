#include <Celo/StringHelp.h>
#include "Command.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>

void Command::feed_stream_with( std::ostream &os, const std::string &inp ) {
    std::ifstream is( inp.c_str() );
    while ( true ) {
        char buffer[ 2048 ];
        int r = is.readsome( buffer, sizeof buffer );
        if ( r <= 0 )
            break;
        // std::cout.write( buffer, r );
        os.write( buffer, r );
    }
}

static std::string chmok( std::string dst ) {
    std::string res;
    for( unsigned i = 0; i < dst.size(); ++i )
        res += dst[ i ] != '/' ? dst[ i ] : '_';
    return res;
}
int Command::exec( const std::string &base, const std::string &_cmd, std::string dst, Vec<std::string> src ) {
    // no change ?
    if ( src.size() ) {
        bool ok = true;
        for( int i = 0; i < src.size(); ++i ) {
            if ( system( ( "cmp " + src[ i ] + " " + src[ i ] + ".old_for_" + chmok( dst ) ).c_str() ) == 0 )
                std::cout << "No changes in " << src[ i ] << " to make " << dst << std::endl;
            else {
                system( ( "cp " + src[ i ] + " " + src[ i ] + ".old_for_" + chmok( dst ) ).c_str() );
                ok = false;
            }
        }
        if ( ok )
            return 0;
    }


    this->cmd = _cmd;
    PRINT( _cmd );
    std::string ouf = base + ".out";
    std::string erf = base + ".err";
    std::string cmd = _cmd + " > " + ouf + " 2> " + erf;
    return_code = system( cmd.c_str() );

    std::ostringstream ss_out;
    std::ostringstream ss_err;
    feed_stream_with( ss_out, ouf );
    feed_stream_with( ss_err, erf );
    out = ss_out.str();
    err = ss_err.str();

    if ( return_code )
        std::cerr << *this << std::endl;

    return return_code;
}

std::ostream &operator<<( std::ostream &os, const Command &cmd ) {
    os << "  ---------------- cmd ----------------\n" << cmd.cmd << "\n";
    os << "  ---------------- ret ----------------\n" << cmd.return_code << "\n";
    os << "  ---------------- out ----------------\n" << cmd.out << "\n";
    os << "  ---------------- err ----------------\n" << cmd.err << "\n";
    return os;
}

