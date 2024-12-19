#ifndef _DYNAMIC_LOADER
#define _DYNAMIC_LOADER

#include <Windows.h>
#include "Core/routes.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"

typedef Router(__cdecl *DLLROUTERFUNC)();
typedef void(__cdecl *DLLSETGLOBALSFUNC)(dotenv dt, enum LOGGING_LEVEL ll);

typedef struct retFromDLL
{
    int found;
    Router r;
} retFromDLL;

retFromDLL getRouterFromDLL(char *lib_name);

#endif