#include "../Sys/UsualStrings.h"
#include "User.h"

User::User( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( inp, rights, watching_sessions ) {
}

User::User( RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( rights, watching_sessions ) {
}

Nstring User::type() const {
    return NSTRING_User;
}
