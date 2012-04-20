#ifndef MODEL_H
#define MODEL_H

#include "SessionSet.h"
#include "RightSet.h"

/**
*/
class Model {
public:
    Model( RightSet rights, SessionSet watching_sessions );
    virtual ~Model();

    Database *db();

    RightSet rights;
    SessionSet watching_sessions;
};

#endif // MODEL_H
