#include "../Database/MapRead.h"
#include "../Sys/UsualStrings.h"
#include "File.h"
#include "Val.h"

File::File( BinInp *inp, RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( inp, rights, watching_sessions  ) {
}

File::File( RightSet rights, SessionSet watching_sessions ) : ModelWithAttr( rights, watching_sessions ) {
}

void File::map_ptr( const MapRead &map_read ) {
    if ( op_id == cur_op_id )
        return;
    op_id = cur_op_id;

    ModelWithAttr::_map_ptr( map_read );

    if ( Val *v = dynamic_cast<Val *>( attr( NSTRING__ptr ) ) ) {
        Model *m = map_read[ reinterpret_cast<Model *>( v->man ) ];
        v->man = reinterpret_cast<SI64>( m );
        if ( m )
            m->map_ptr( map_read );
    }
}

Nstring File::type() const {
    return NSTRING_File;
}
