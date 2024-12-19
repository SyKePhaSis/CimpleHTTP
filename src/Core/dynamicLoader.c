#include <Windows.h>
#include "Core/dynamicLoader.h"
#include "Core/routes.h"
#include "Utils/logger.h"

retFromDLL getRouterFromDLL(char *lib_name)
{
    HINSTANCE hDLL;
    DLLROUTERFUNC RouterFunc;
    DLLSETGLOBALSFUNC GlobalsFunc;
    retFromDLL ret;
    ret.found = 0;
    hDLL = LoadLibrary(TEXT(lib_name));
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
            return ret;
        }
        logWarning("Couldn't get Router Function");
        logError("Error in dynamic loading: %d", GetLastError());
        return ret;
    }
    logWarning("Couldn't open library");
    logError("Error in dynamic loading: %d", GetLastError());
    return ret;
}