#pragma once

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "Core/routes.h"
#include "Core/dataHandling.h"
#include "Core/returnReqs.h"
#include "Core/returnReqs.h"
#include "Utils/macros.h"
#include "Utils/fileHandling.h"
#include "Utils/httpCreator.h"

void getIndex(SOCKET *s, request req);
void postIndex(SOCKET *s, request req);
void putIndex(SOCKET *s, request req);
void deleteIndex(SOCKET *s, request req);

Router getIndexRoutes();
