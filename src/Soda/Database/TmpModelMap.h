#ifndef TMPMODELMAP_H
#define TMPMODELMAP_H

#include "Database.h"

/**
*/
struct TmpModelMap {
    typedef std::map<ST,Model *> TM;

    inline Model *operator()( ST ptr_model, Session *session ) const {
        if ( ptr_model & 3 ) {
            TM::const_iterator iter = tmp_map.find( ptr_model );
            return iter != tmp_map.end() ? iter->second : 0;
        }
        return session->db->model_allocator.check( reinterpret_cast<Model *>( ptr_model ) );
    }

    inline Model *operator()( Model *ptr_model, Session *session ) const {
        return operator()( (ST)ptr_model, session );
    }

    TM tmp_map;
};

#endif // TMPMODELMAP_H
