#include "Core/routes.h"
#include "Core/assetRouting.h"

void defineRoutes()
{
    // Define Routes
    defineRoute(GET, "/", getIndex);
    defineRoute(GET, "*", get404);

    // Define Asset Routes
    defineAssetRoutes();
}