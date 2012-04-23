#ifndef TMPMODELMAP_H
#define TMPMODELMAP_H

#include "Database.h"

/**
*/
struct TmpModelMap {
    inline Model *operator()( ST ptr_model, Session *session ) const {
        if ( ptr_model & 3 ) {
            std::map<ST,Model *>::const_iterator iter = tmp_map.find( ptr_model );
            return iter != tmp_map.end() ? iter->second : 0;
        }
        return session->db->model_allocator.check( reinterpret_cast<Model *>( ptr_model ) );
    }

    std::map<ST,Model *> tmp_map;
};

#endif // TMPMODELMAP_H
