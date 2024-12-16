#include "DataTypes/types.h"

GenericArray *getGenericArray();
void addToGenericArray(GenericArray *ga, void *item, ITEM_TYPE it);
void removeFromGenericArray(GenericArray *ga, long index);
void *getFromGenericArray(GenericArray *ga, long index);
void *getTypeFromGenericArray(GenericArray *ga, long index);
void freeGenericArray(GenericArray *ga);