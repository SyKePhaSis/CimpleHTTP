#include "Core/routes.h"
#include "Core/signalHandling.h"
#include "Core/filtering.h"
#include "Core/routes.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"
#include "server.h"
#include "routes.h"
#include <WinSock2.h>
#include <signal.h>

int main()
{
    setLogLevel(Info);
    init_signals();
    parseDotEnv();
    initializeWhitelist();
    initRoutingTable();
    defineRoutes();
    printRoutes();
    startServer();
    return 0;
}