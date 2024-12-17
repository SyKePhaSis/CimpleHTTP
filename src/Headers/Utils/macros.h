#ifndef _CIMPLE_MACROS
#define _CIMPLE_MACROS

#include "DataTypes/types.h"

#define UNUSED(x) __attribute__((unused)) x

#define TYPE_PAIR(item, type) getTypePair(item, type)

#endif