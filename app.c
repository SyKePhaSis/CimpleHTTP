#include<WinSock2.h>
#include"Core/routes.h"

void defineRoutes()
{
    // Define Routes
    defineRoute(GET, "/", getIndex);
    defineRoute(GET, "*", get404);
}