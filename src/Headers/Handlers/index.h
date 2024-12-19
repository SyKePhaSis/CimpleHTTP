#pragma once

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "Core/routes.h"
#include "Core/dataHandling.h"
#include "Core/returnReqs.h"
#include "Core/returnReqs.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"
#include "Utils/macros.h"
#include "Utils/fileHandling.h"
#include "Utils/httpCreator.h"

#if defined(_WIN32)
#define CIAPI __declspec(dllexport)
#else
#define CIAPI
#endif

CIAPI void getIndex(SOCKET *s, request req);
CIAPI void postIndex(SOCKET *s, request req);
CIAPI void putIndex(SOCKET *s, request req);
CIAPI void deleteIndex(SOCKET *s, request req);

CIAPI void setGlobals(dotenv dt, enum LOGGING_LEVEL ll)
{
    setLogLevel(ll);
    setDotEnv(dt);
}

CIAPI Router getIndexRoutes();
