#include "../Database/TmpModelMap.h"
#include "../Database/Session.h"
#include "../Sys/BinOut.h"
#include "Model.h"
#include <sstream>

PI64 Model::cur_op_id = 0;

Model::Model( RightSet rights, SessionSet watching_sessions ) : watching_sessions( watching_sessions ), rights( rights ), op_id( 0 ) {
    mod_session = 0;
    in_mod_list = false;
}

Model::~Model() {
}

bool Model::write_njs( Stream &nut, Stream &uut, Session *session ) const {
    if ( not rights.has( session->user, RD ) )
        return false;
    if ( not watching_sessions.has( session ) ) {
        watching_sessions << session;
        if ( not write_ujs( nut, uut, session ) )
            return false;
        nut << "_w(" << this << ",new " << js_type() << ");\n";
    }
    return true;
}

bool Model::write_nsr( BinOut &nut, BinOut &uut, Session *session ) const {
    PRINT( rights.has( session->user, RD ) );
    //if ( not rights.has( session->user, RD ) )
    //    return false;
    if ( not watching_sessions.has( session ) ) {
        watching_sessions << session;
        if ( not write_usr( nut, uut, session ) )
            return false;
        nut << 'N' << PI64( this ) << (std::string)type();
    }
    return true;
}

int Model::nb_attr() const { return 0; }
Model *Model::attr( int index ) const { return 0; }
Model *Model::attr( Nstring name ) const { return 0; }
Model *Model::attr( StringBlk name ) const { return 0; }

Model::operator SI64() const { return 0; }
Model::operator String() const { return String(); }

bool Model::equal( StringBlk name ) const { TODO; std::cout << type() << std::endl; return false; }

Nstring Model::js_type() const { return _js_type ? _js_type : type(); }

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
