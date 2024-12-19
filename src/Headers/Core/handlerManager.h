#ifndef _HANDLER_MANAGER
#define _HANDLER_MANAGER

#include "Core/fileWatcher.h"
#include "DataTypes/dynamicArray.h"

void loadHandlers();
WatcherObject *createHandlerWatcher();
void reloadHandler(char *handler);

#endif