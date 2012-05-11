#include "../Database/TmpModelMap.h"
#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "../Sys/BinOut.h"
#include "../Sys/BinInp.h"
#include "../Sys/Stream.h"
#include "Path.h"

Path::Path( RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( rights, watching_sessions ) {
    path = watching_sessions->new_file( 0 );
}

Path::Path( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( inp, rights, watching_sessions ) {
    *inp >> path;
}

void Path::write_dmp( BinOut &out ) const {
    ModelWithAttr::write_dmp( out );
    out << path;
}

Nstring Path::type() const {
    return NSTRING_Path;
}

int Path::type_dump() const {
    return _Path;
}

