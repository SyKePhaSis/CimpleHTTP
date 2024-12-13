#include "Core/routes.h"
#include "Core/assetRouting.h"
#include "Handlers/index.h"

void defineRoutes()
{
    // Define Routes
    defineRoute(GET, "*", get404);

    // Define Routers
    defineRouter(getIndexRoutes());

    // Define Asset Routes
    defineAssetRoutes();
}