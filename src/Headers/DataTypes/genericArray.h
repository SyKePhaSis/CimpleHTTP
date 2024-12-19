#include "DataTypes/types.h"

GenericArray *getGenericArray();
void addToGenericArray(GenericArray *ga, void *item, ITEM_TYPE it);
void addUnknownToGenericArray(GenericArray *ga, void *item, size_t size);
void removeFromGenericArray(GenericArray *ga, long index);
void *getFromGenericArray(GenericArray *ga, long index);
void *getTypeFromGenericArray(GenericArray *ga, long index);
void freeGenericArray(GenericArray *ga);