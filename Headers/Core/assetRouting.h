#pragma once
#ifndef _ASSET_ROUTING
#define _ASSET_ROUTING

#define CSS_ASSET_FOLDER "./WebPage/assets/css"
#define CSS_ASSET_ROUTE "/assets/css/"

typedef struct Assets
{
    size_t size;
    char **array;
} Assets;

Assets getAssets();
void defineAssetRoutes();
void printFolderContents();
void cleanUp(Assets *assets);

#endif