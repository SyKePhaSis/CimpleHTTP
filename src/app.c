#include "Core/routes.h"
#include "Core/signalHandling.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"
#include "server.h"
#include "routes.h"
#include <WinSock2.h>
#include <signal.h>

int main()
{
    setLogLevel(Connection);
    init_signals();
    parseDotEnv();
    defineRoutes();
    printRoutes();
    startServer();
    return 0;
}