#include "CppSession.h"

CppSession::CppSession( Database *db, User *user, int num_inst ) : Session( db, user ), num_inst( num_inst ) {
}
