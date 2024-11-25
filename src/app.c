#include "Core/routes.h"
#include "Core/signalHandling.h"
#include "Core/filtering.h"
#include "Utils/logger.h"
#include "Utils/dotenv.h"
#include "server.h"
#include "routes.h"
#include <WinSock2.h>
#include <signal.h>
#include <assert.h>

int main()
{
    setLogLevel(All);
    init_signals();
    parseDotEnv();
    initializeWhitelist();
    defineRoutes();
    printRoutes();
    startServer();
    return 0;
}