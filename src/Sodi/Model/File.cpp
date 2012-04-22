#include "../Sys/UsualStrings.h"
#include "File.h"

File::File( RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( rights, watching_sessions ) {
}

Nstring File::type() const {
    return NSTRING_File;
}
