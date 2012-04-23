#ifndef MAPREAD_H
#define MAPREAD_H

#include "../Model/Model.h"

/**
  Used to read file with "bad" (old) pointer refs
*/
struct MapRead {
    Model *operator[]( Model *m ) const {
        std::map<Model *,Model *>::const_iterator res = m_map.find( m );
        return res != m_map.end() ? res->second : 0;
    }

    Nstring operator[]( Nstring m ) const {
        std::map<Nstring,Nstring>::const_iterator res = n_map.find( m );
        return res != n_map.end() ? res->second : Nstring();
    }

    RightSet operator[]( RightSet m ) const {
        std::map<RightSet,RightSet>::const_iterator res = r_map.find( m );
        return res != r_map.end() ? res->second : RightSet();
    }

    std::map<Model  *,Model  *> m_map;
    std::map<Nstring ,Nstring > n_map;
    std::map<RightSet,RightSet> r_map;
};

#endif // MAPREAD_H
