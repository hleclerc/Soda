#include "../Database/TmpModelMap.h"
#include "../Database/Session.h"
#include "Model.h"
#include <sstream>

PI64 Model::cur_op_id = 0;

Model::Model( RightSet rights, SessionSet watching_sessions ) : watching_sessions( watching_sessions ), rights( rights ), op_id( 0 ) {
    mod_session = 0;
    in_mod_list = false;
}

Model::~Model() {
}

bool Model::write_njs( Stream &out, int var, Session *session ) const {
    if ( rights.has( session->user, RD ) ) {
        if ( watching_sessions.has( session ) )
            out << "var v_" << var << " = FileSystem._objects[ " << this << " ];\n";
        else {
            watching_sessions << session;
            if ( not _write_njs( out, var, session ) ) {
                out << "var v_" << var << " = undefined;\n";
                return false;
            }
            out << "v_" << var << "._server_id = " << this << ";\n";
            out << "FileSystem._objects[ " << this << " ] = v_" << var << ";\n";
        }
        return true;
    }
    out << "var v_" << var << " = undefined;\n";
    return false;
}

int Model::nb_attr() const { return 0; }
Model *Model::attr( int index ) const { return 0; }
Model *Model::attr( Nstring name ) const { return 0; }
Model *Model::attr( StringBlk name ) const { return 0; }

Model::operator SI64() const { return 0; }
Model::operator String() const { return String(); }

bool Model::equal( StringBlk name ) const { TODO; std::cout << type() << std::endl; return false; }

const Database *Model::db() const { return *watching_sessions; }
Database *Model::db() { return *watching_sessions; }

void Model::map_ptr( const MapRead &map_read ) {
    if ( op_id != cur_op_id ) {
        op_id = cur_op_id;
        _map_ptr( map_read );
    }
}

void Model::rm_inactive_sessions() {
    for( int i = 0; i < watching_sessions.size(); ++i ) {
        Session *w = watching_sessions[ i ];
        if ( w->inactive ) {
            watching_sessions >> w;
            return rm_inactive_sessions();
        }
    }
}
