#include "Core/routes.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"
#include "server.h"
#include "routes.h"
#include <WinSock2.h>

int main()
{
    setLogLevel(Connection);
    parseDotEnv();
    defineRoutes();
    printRoutes();
    startServer();
    return 0;
}