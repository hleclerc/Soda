#ifndef SECUREALLOC_H
#define SECUREALLOC_H

#include "AllocFactory.h"
#include "_SecureAlloc.h"

/**
*/
class SecureAlloc : public AllocFactory<_SecureAlloc> {
public:
};

#endif // SECUREALLOC_H
