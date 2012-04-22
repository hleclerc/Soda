#ifndef USUALSTRINGS_H
#define USUALSTRINGS_H

#include "Nstring.h"

#define US( N ) extern Nstring NSTRING_##N;
#include "UsualStrings_Decl.h"
#undef US

#endif // USUALSTRINGS_H
