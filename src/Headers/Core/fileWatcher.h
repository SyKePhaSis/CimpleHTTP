#ifndef _FILE_WATCHER
#define _FILE_WATCHER

#include <windows.h>

typedef struct retCheckWatcher
{
    char *fileName;
    int updated;
} retCheckWatcher;

typedef struct WatcherObject
{
    HANDLE hDir;
    OVERLAPPED overlapped;
    unsigned char *buff;
    size_t buffsize;
    void (*updateFunc)(char *filename);
} WatcherObject;

WatcherObject *getDirectoryWatcher(char *path);
retCheckWatcher checkForUpdate(WatcherObject *wo);
void RefreshDirectory();

#endif
