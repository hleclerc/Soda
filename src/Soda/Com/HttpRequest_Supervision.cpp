#include "HttpRequest_Supervision.h"
#include <Celo/StreamSep.h>
#include <sstream>

HttpRequest_Supervision::HttpRequest_Supervision( int fd, SodaLoop *loop ) : EventObj_HttpRequest( fd ), loop( loop ) {
}

void HttpRequest_Supervision::index() {
    std::ostringstream ss;
    StreamSepMaker<std::ostream> sl( ss );

    // header
    sl.end = "\n";
    sl << "HTTP/1.0 200 OK";
    sl << "Content-Type: text/html";
    sl << "";
    sl << "<html>";
    sl << "  <head>";
    sl << "    <script type='text/javascript'>";
    sl << "        my_xml_http_request = function() {";
    sl << "            if ( window.XMLHttpRequest ) { return new XMLHttpRequest(); }";
    sl << "            if ( window.ActiveXObject  ) { return new ActiveXObject('Microsoft.XMLHTTP'); }";
    sl << "            return alert('Votre navigateur ne supporte pas les objets XMLHTTPRequest..');";
    sl << "        }";
    sl << "        send = function( url, data ) {";
    sl << "            var xhr_object = this.my_xml_http_request();";
    sl << "            xhr_object.open( 'GET', url, true );";
    sl << "            xhr_object.onreadystatechange = function() {";
    sl << "                if ( this.readyState === 4 && this.status === 200 ) {";
    sl << "                        eval( this.responseText );";
    sl << "                }";
    sl << "            }";
    sl << "            return xhr_object.send( data );";
    sl << "        }";
    sl << "    </script>";
    sl << "  </head>";
    sl << "  <body>";


    // data
    sl.end = "<br/>\n";
    // sl << "Date last build: " << soda->update_dl.dl_date;
    sl << "<button onclick='send(\"/reload\");');'>reload pages</a>";
    sl << "<button onclick='send(\"/exit\"  );');'>exit server </a>";

    // footer
    sl.end = "\n";
    sl << "  </body>";
    sl << "</html>";

    // send
    send_str( ss.str().c_str() );
}
