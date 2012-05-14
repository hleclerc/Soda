#include "../Database/TmpModelMap.h"
#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "File.h"
#include "Val.h"

File::File( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( inp, rights, watching_sessions  ) {
}

File::File( RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( rights, watching_sessions ) {
}

Nstring File::type() const {
    return NSTRING_File;
}
