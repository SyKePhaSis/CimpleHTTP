#ifndef _RET_REQS
#define _RET_REQS

#include "Core/httpParser.h"
#include <WinSock2.h>

void view_or_404(SOCKET *s, request req, char *path);
void view_404(SOCKET *s, request req);

#endif
