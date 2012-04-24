#include "sys/Stream.h"
#include "ErrorList.h"

ErrorList::ErrorList() {
}

ErrorList &ErrorList::operator<<( String error ) {
    std::cout << error << std::endl;
    errors << error;
    return *this;
}
