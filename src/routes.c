#include "Core/routes.h"
#include "Core/assetRouting.h"
#include "Core/handlerManager.h"

void defineRoutes()
{
    // Define Routes
    defineRoute(GET, "*", get404);

    // Define Routers
    // defineRouter(getRouterFromDLL("./build/objects/Handlers/index.dll"));
    loadHandlers();

    // Define Asset Routes
    defineAssetRoutes();
}