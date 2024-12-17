#include "DataTypes/types.h"

TypePair getTypePair(void *item, ITEM_TYPE it)
{
    TypePair tp = {.item = item, .it = it};
    return tp;
}