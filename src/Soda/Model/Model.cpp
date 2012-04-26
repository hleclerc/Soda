#include "../Database/TmpModelMap.h"
#include "../Database/Session.h"
#include "Model.h"
#include <sstream>

PI64 Model::cur_op_id = 0;

Model::Model( RightSet rights, SessionSet watching_sessions ) : watching_sessions( watching_sessions ), rights( rights ), op_id( 0 ) {
    mod_session = 0;
}

Model::~Model() {
}

Model::operator SI64() const { return 0; }
Model::operator FP64() const { return 0; }

Model::operator std::string() const {
    std::ostringstream ss;
    write_str( ss );
    return ss.str();
}

void Model::write_njs( Stream &out, int var, Session *session ) const {
    if ( watching_sessions.has( session ) )
        out << "var v_" << var << " = FileSystem._objects[ " << this << " ];\n";
    else {
        watching_sessions << session;
        _write_njs( out, var, session );
        out << "v_" << var << "._server_id = " << this << ";\n";
        out << "FileSystem._objects[ " << this << " ] = v_" << var << ";\n";
    }
}

int Model::nb_attr() const { return 0; }
Model *Model::attr( int index ) const { return 0; }
Model *Model::attr( Nstring name ) const { return 0; }
Model *Model::attr( StringBlk name ) const { return 0; }

bool Model::equal( StringBlk name ) const { TODO; std::cout << type() << std::endl; return false; }

bool Model::_set( StringBlk data ) { TODO; std::cout << type() << std::endl; return false; }
bool Model::_set( Model *m ) { TODO; std::cout << type() << std::endl; return false; }
bool Model::_set( SI64 val ) { TODO; std::cout << type() << std::endl; return false; }
bool Model::_set( FP64 val ) { TODO; std::cout << type() << std::endl; return false; }

bool Model::_add_attr( Session *s, StringBlk name, Model *m ) { TODO; std::cout << type() << std::endl; return false; }
bool Model::_add_attr( Session *s, Nstring name, Model *m ) { TODO; std::cout << type() << std::endl; return false; }

bool Model::_push( Model *m ) { TODO; std::cout << type() << std::endl; return false; }

const Database *Model::db() const { return *watching_sessions; }
Database *Model::db() { return *watching_sessions; }

void Model::rm_inactive_sessions() {
    for( int i = 0; i < watching_sessions.size(); ++i ) {
        Session *w = watching_sessions[ i ];
        if ( w->inactive ) {
            watching_sessions >> w;
            return rm_inactive_sessions();
        }
    }
}
