#include "Database.h"

Database::Database( const char *filename ) {
    *session_set_list = this;
}

Database::~Database() {
}
