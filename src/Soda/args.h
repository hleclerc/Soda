DESCRIPTION( "SODA, the Synchronized Object DAtabase" );

SARG( 'b', base_dir, "base directory of files to be served (/ in http requests)", 0 );
SARG(  0 , db_file , "file name of the database", "dump.db" );
SARG(  0 , db_dir  , "name of the database file directory (for bulk data)", "data.db" );
BARG( 'v', verbose , "will give more information", false );

BARG( 'L', launch_browser_super, "after the server start, launch a browser on the supervision page", false );
BARG( 'l', launch_browser      , "after the server start, launch a browser on the public page (see -s)", false );
SARG( 's', start_page          , "start page for the browser on the public data", "/" );
SARG( 't', title_page          , "title of the page used for xdotool", 0 );

SARG( 'P', super_port, "http port for supervision" , "8889" );
SARG( 'p', port      , "http port for public pages", "8888" );
SARG( 'q', soda_port , "port for binary public communication", "8890" );

SARG( 'c', compilation_dir, "compilation dir (generation for static pages, ...)", "compilations" );
SARG(  0 , sipe_executable, "path to the sipe executable", ( INSTALL_DIR "/ext/Sipe/sipe" ) );
SARG(  0 , cxx            , "compiler"         , "g++" );
SARG(  0 , ld             , "linker"           , "g++" );
SARG(  0 , objcopy        , "objcopy utility"  , "objcopy" );
SARG(  0 , obj_format     , "object format (used by objcopy)"  , ( sizeof(void *) == 4 ? "elf32-i386" : "elf64-x86-64" ) );

IARG(  0 , timeout_chan   , "timeout in s to respawn push channels (if no websocket)", 30 );
