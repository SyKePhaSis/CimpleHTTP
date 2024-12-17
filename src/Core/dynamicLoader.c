#include <Windows.h>
#include "Core/dynamicLoader.h"
#include "Core/routes.h"
#include "Utils/logger.h"

Router getRouterFromDLL(char *lib_name)
{
    HINSTANCE hDLL;
    DLLROUTERFUNC RouterFunc;
    Router router;
    hDLL = LoadLibrary(TEXT(lib_name));
    if (hDLL)
    {
        RouterFunc = (DLLROUTERFUNC)GetProcAddress(hDLL, TEXT("getIndexRoutes"));
        if (RouterFunc)
        {
            router = RouterFunc();
            return router;
        }
        logWarning("Couldn't get Router Function");
        logError("Error in dynamic loading: %d", GetLastError());
        return;
    }
    logWarning("Couldn't open library");
    logError("Error in dynamic loading: %d", GetLastError());
}