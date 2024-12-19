#include <Windows.h>
#include "Core/dynamicLoader.h"
#include "Utils/memmory.h"
#include "Utils/fileHandling.h"
#include "Core/routes.h"
#include "Utils/logger.h"
#include "DataTypes/dictionary.h"
#include "DataTypes/types.h"

Dict *dllDict;

void initDllDict()
{
    dllDict = createDict();
}

retFromDLL getRouterFromDLL(char *lib_name)
{
    HINSTANCE hDLL;
    DLLROUTERFUNC RouterFunc;
    DLLSETGLOBALSFUNC GlobalsFunc;
    retFromDLL ret;
    char *file_name = getFileName(lib_name);
    char *dll_paths = getEnvValue("HOT_RELOAD_LOCATION");
    char *hot_file = allocate(strlen(file_name) + strlen(dll_paths) + 1);
    strcpy(hot_file, dll_paths);
    strcat(hot_file, file_name);
    ret.found = 0;
    ret.file_path = allocate(strlen(lib_name) + 1);
    strcpy(ret.file_path, lib_name);
    logInfo("%s : %s", lib_name, hot_file);
    CopyFile(lib_name, hot_file, 0);
    hDLL = LoadLibrary(TEXT(hot_file));
    deallocate(hot_file);
    ret.hDLL = hDLL;
    if (hDLL)
    {
        GlobalsFunc = (DLLSETGLOBALSFUNC)GetProcAddress(hDLL, TEXT("setGlobals"));
        if (!GlobalsFunc)
        {
            logError("Could't find Handlers DLL setGlobals function");
            return ret;
        }
        GlobalsFunc(*getDotEnv(), getLogLevel());
        RouterFunc = (DLLROUTERFUNC)GetProcAddress(hDLL, TEXT("getIndexRoutes"));
        if (RouterFunc)
        {
            ret.r = RouterFunc();
            ret.found = 1;
            addUnknownToDict(dllDict, lib_name, &ret, sizeof(retFromDLL));
            return ret;
        }
        logWarning("Couldn't get Router Function");
        logError("Error in dynamic loading: %d", GetLastError());
        FreeLibrary(hDLL);
        return ret;
    }
    logWarning("Couldn't open library");
    logError("Error in dynamic loading: %d", GetLastError());
    return ret;
}

retFromDLL reloadDLL(char *path)
{
    logInfo("Fetching DLL to free");
    retFromDLL *val = (retFromDLL *)getValueFromDict(dllDict, path);
    if (val == NULL)
    {
        logError("Couldn't get old DLL - Aborting");
    }
    FreeLibrary(val->hDLL);
    logInfo("Freed Old DLL");

    DLLROUTERFUNC RouterFunc;
    DLLSETGLOBALSFUNC GlobalsFunc;
    val->hDLL = LoadLibrary(TEXT(val->file_path));
    val->found = 0;
    if (val->hDLL)
    {
        GlobalsFunc = (DLLSETGLOBALSFUNC)GetProcAddress(val->hDLL, TEXT("setGlobals"));
        if (!GlobalsFunc)
        {
            logError("Could't find Handlers DLL setGlobals function");
            return *val;
        }
        GlobalsFunc(*getDotEnv(), getLogLevel());
        RouterFunc = (DLLROUTERFUNC)GetProcAddress(val->hDLL, TEXT("getIndexRoutes"));
        if (RouterFunc)
        {
            val->r = RouterFunc();
            val->found = 1;
            return *val;
        }
        logWarning("Couldn't get Router Function");
        logError("Error in dynamic loading: %d", GetLastError());
        removeFromDict(dllDict, val->r.path);
        FreeLibrary(val->hDLL);
        return *val;
    }
    logWarning("Couldn't open library");
    logError("Error in dynamic loading: %d", GetLastError());
    removeFromDict(dllDict, val->r.path);
    return *val;
}

void closeDLL(char *path)
{
    void *val = getValueFromDict(dllDict, path);
    if (val != NULL)
    {
        removeFromDict(dllDict, path);
    }
    FreeLibrary(((retFromDLL *)val)->hDLL);
}