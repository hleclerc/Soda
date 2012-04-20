#include "Model.h"

Model::Model( RightSet rights, SessionSet watching_sessions ) : rights( rights ), watching_sessions( watching_sessions ) {
}

Model::~Model() {
}

Database *db() {
    return *watching_sessions;
}
