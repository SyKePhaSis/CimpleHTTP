
#include "Core/handlerManager.h"
#include "Core/dynamicLoader.h"
#include "Core/fileWatcher.h"
#include "Utils/dotenv.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "DataTypes/dictionary.h"
#include "DataTypes/types.h"

#include <windows.h>
#include <strsafe.h>
#include <winnt.h>

/* REMOVE */

#include "Utils/macros.h"

/* VARIABLES */

Dict *handlersDict;

/* HELPER FUNCTIONS */

void getHandlerPaths()
{
    WIN32_FIND_DATA ffd;
    char *fpath = getEnvValue("HANDLERS_LOCATION");
    if (!fpath)
    {
        logError("Couldn't get Handlers file path - stopped loading");
        return;
    }
    TCHAR *szDir = allocate(sizeof(PTCHAR) * (strlen(fpath) + 3));
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    StringCchCopy(szDir, strlen(fpath) + 3, fpath);
    StringCchCat(szDir, strlen(fpath) + 3, TEXT("/*"));
    logSuccess("Handler Search Path: %s", szDir);

    hFind = FindFirstFile(szDir, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        logError("Couldn't get handle to Handlers Folder - Aborting");
        deallocate(szDir);
    }

    logInfo("------ %s HANDLERS DIRECTORY ------", fpath);
    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            size_t s = strlen(fpath) + strlen(ffd.cFileName) + 2;
            char *tmp = allocate(s);
            StringCchCopy(tmp, s, fpath);
            StringCchCat(tmp, s, "/");
            StringCchCat(tmp, s, ffd.cFileName);
            addToDict(handlersDict, ffd.cFileName, tmp, STR);
            logInfo("<FILE>  %s  ", ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        logError("Windows error occured while loading Handlesr: %d", dwError);
    }
    FindClose(hFind);
    deallocate(szDir);
}

/* EXPORTED FUNCTIONS */

void loadHandlers()
{
    handlersDict = createDict();
    initDllDict();
    getHandlerPaths();
    Array *keys = getArray(CHAR_ARR);
    getDictKeys(handlersDict, keys);
    Iterator *it = createIterator(keys);
    while (iteratorHasNext(it))
    {
        char *hFname = (char *)iteratorGetNext(it);
        char *hPath = (char *)getValueFromDict(handlersDict, hFname);
        logInfo("handler path: %s", hPath);
        retFromDLL ret = getRouterFromDLL(hPath);
        if (ret.found)
        {
            defineRouter(ret.r);
        }
    }
    iteratorCleanup(it);
}

WatcherObject *createHandlerWatcher()
{
    char *dpath = getEnvValue("HANDLERS_LOCATION");
    if (!dpath)
    {
        logError("Couldn't get Handlers file path - stopped loading");
        return NULL;
    }
    WatcherObject *wo = getDirectoryWatcher(dpath);
    return wo;
}

void reloadHandler(char *dll_name)
{
    char *fullpath = getValueFromDict(handlersDict, dll_name);
    deallocate(dll_name);
    if (fullpath)
    {
        logSuccess("Found fullpath to updated dll: %s", fullpath);
        retFromDLL ret = reloadDLL(fullpath);
        logSuccess("Reloaded DLL");
        removeRouteFamily(ret.r.path);
        if (ret.found)
        {
            defineRouter(ret.r);
        }
    }
}
