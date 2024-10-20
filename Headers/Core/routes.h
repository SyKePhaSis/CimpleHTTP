#pragma once
#ifndef _ROUTES
#define _ROUTES

#include<WinSock2.h>
#include"Core/httpParser.h"
#include"Core/RoutingTable.h"

void getIndex(SOCKET *csock, request req);
void get404(SOCKET *csock, request req);
void defineRoute(enum METHODS method, char* path, void (*get)(SOCKET *cscok, request req));
RouteTable* getRoutingTable();

#endif