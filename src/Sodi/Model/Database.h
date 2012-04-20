#ifndef DATABASE_H
#define DATABASE_H

#include "../Sys/NstringList.h"
#include "Model.h"
class Directory;

/**

*/
class Database {
public:
    enum { FILE_VERSION = 1 };

    Database( const char *filename = 0 );
    ~Database();

    SessionSet::List session_set_list;
    RightSet::List right_set_list;
    NstringList nstring_list;
    Directory *root_dir;
    User *root_usr;
};

#endif // DATABASE_H
