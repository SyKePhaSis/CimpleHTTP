#ifndef _BUILT_IN_HANDLERS
#define _BUILT_IN_HANDLERS

#include <WinSock2.h>
#include "Core/httpParser.h"

void built_in_unsuported_method(SOCKET *csock, request req);
void built_in_internal_server_error(SOCKET *csock, request req);

#endif