#ifndef _DYNAMIC_LOADER
#define _DYNAMIC_LOADER

#include <Windows.h>
#include "Core/routes.h"

typedef Router(__cdecl *DLLROUTERFUNC)();

Router getRouterFromDLL(char *lib_name);

#endif