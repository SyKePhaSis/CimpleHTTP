#include "Core/signalHandling.h"
#include "Utils/logger.h"
#include <signal.h>
#include <stdlib.h>

static void handleSIGINT()
{
    logSuccess("Gracefully Exiting...");
    exit(0);
}

static void handleSIGILL()
{
    logError("Illegal instruction - Invalid function image error occured!");
    exit(-1);
}

void init_signals()
{
    signal(SIGINT, handleSIGINT);
    signal(SIGILL, handleSIGILL);
}