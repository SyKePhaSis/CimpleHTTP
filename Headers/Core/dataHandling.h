#pragma once
#ifndef _DATA_HANDLER
#define _DATA_HANDLER

#include <WinSock2.h>

int sendData(SOCKET *csock, void *buf, size_t size);

#endif