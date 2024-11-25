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

static void handleSIGABRT()
{
    logError("abnormal termination triggered by abort call");
    exit(-1);
}

static void handleSIGSEGV()
{
    logError("segment violation");
    exit(-1);
}

static void handleSIGFPE()
{
    logError("loating point exception");
    exit(-1);
}

void init_signals()
{
    signal(SIGINT, handleSIGINT);
    signal(SIGILL, handleSIGILL);
    signal(SIGABRT, handleSIGABRT);
    signal(SIGSEGV, handleSIGSEGV);
    signal(SIGFPE, handleSIGFPE);
}