#include <Celo/StaticFileData.h>
#include <Celo/StringHelp.h>
#include "ServerLoop.h"
#include "UpdateDl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <dlfcn.h>
#include <stdio.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name dl
#pragma lnk_flag -Wl,--export-dynamic
#endif

// class
UpdateDl::UpdateDl( ServerLoop *l ) : l( l ) {
    has_update = false;
}

void UpdateDl::run() {
    if ( mutex.try_lock() )
        return;
    AutoMutexFree af( mutex );

    // base files
    mkdir( l->compilation_dir, 0777 );
    std::string dir( l->compilation_dir ), base = dir + "/dl_req";
    std::ofstream sipe_file( ( base + ".sipe" ).c_str() );
    std::ofstream data_file( ( base + ".data" ).c_str() );
    std::ofstream cpp_file ( ( base + ".cpp" ).c_str() );

    if ( l->base_dir ) {
        if ( l->verbose )
            std::cerr << "generating static information from '" << l->base_dir << "'." << std::endl;

        StaticFileData sd;
        sd.exec( l->base_dir );

        // .sipe
        sipe_file << "static_gurl = \n";
        for( unsigned i = 0; i < sd.info.size(); ++i )
            sipe_file << "    ( '" << sd.info[ i ].url << " ' { send_cst( _data + "
                      << sd.info[ i ].offset << ", "
                      << sd.info[ i ].length << " ); } )" << ( i + 1 < sd.info.size() ? " |\n" : "\n" );

        // .data
        data_file.write( sd.data.data(), sd.data.size() );
    } else if ( l->verbose )
        std::cerr << "No base dir => no served files !" << std::endl;

    Command::feed_stream_with( sipe_file, INSTALL_DIR "/src/Soda/Com/HttpRequest_Public_parser.sipe" );

    cpp_file << "#include <Soda/Com/HttpRequest_Public.h>\n";
    cpp_file << "#include <Celo/Listener_Factory.h>\n";
    cpp_file << "\n";
    cpp_file << "extern \"C\" { extern char _binary_compilations_dl_req_data_start; }\n";
    cpp_file << "const char *_data = &_binary_compilations_dl_req_data_start;\n";
    cpp_file << "\n";
    cpp_file << "struct _HttpRequest : HttpRequest_Public {\n";
    cpp_file << "    _HttpRequest( int fd, ServerLoop *loop ) : HttpRequest_Public( fd, loop ) {} \n";
    cpp_file << "    #define SIPE_CHARP char *\n";
    cpp_file << "    #define SIPE_CLASS\n";
    cpp_file << "    #include \"dl_req.h\"\n";
    cpp_file << "};\n";
    cpp_file << "\n";
    cpp_file << "extern \"C\" void _exec( Listener_WithLaunch *res ) {\n";
    cpp_file << "    new( res ) Listener_Factory<_HttpRequest,ServerLoop>( HttpRequest_Public::VtableOnly() );\n";
    cpp_file << "}\n";

    // generation .sipe -> .h
    sipe_file.close();
    data_file.close();
    cpp_file .close();

    std::string cpp_flags = " -fPIC";
    cpp_flags += " -I" INSTALL_DIR "/src";
    cpp_flags += " -I" INSTALL_DIR "/ext/Celo/src";
    cpp_flags += " -I" INSTALL_DIR "/ext/PrepArg/src";

    // compilation .cpp + .data -> .so
    if ( sipe_cmd.exec( base, std::string( l->sipe_executable ) + " -o " + base + ".h " + base + ".sipe", base + ".h ", base + ".sipe" ) ) return;
    if ( comp_cmd.exec( base, std::string( l->cxx ) + cpp_flags + " -Wall -g3 -c -o " + base + "_cpp.o " + base + ".cpp"/*, base + "_cpp.o ", base + ".cpp"*/ ) ) return;
    if ( mobj_cmd.exec( base, std::string( l->objcopy ) + " -B i386 -I binary -O " + l->obj_format + " " + base + ".data " + base + "_data.o"/*, base + "_data.o ", base + ".data"*/ ) ) return;
    if ( link_cmd.exec( base, std::string( l->ld ) + " -shared " + base + "_cpp.o " + base + "_data.o " + " -o " + base + ".so "/*,
                        base + ".so ", Vec<std::string>( base + "_cpp.o", base + "_data.o" )*/ ) ) return;

    // dlopen
    dlopen( 0, RTLD_GLOBAL );
    dl_req = dlopen( ( base + ".so" ).c_str(), RTLD_LAZY );
    if ( not dl_req ) {
        dl_error = dlerror();
        std::cerr << "dlopen: " << dl_error << std::endl;
        return;
    }

    time_t t = time( 0 );
    char *d = ctime( &t );
    date = d;
    dl_error.clear();

    // execution
    sy_req = (TF *)dlsym( dl_req, "_exec" );
    if ( sy_req )
        has_update = true;
}

