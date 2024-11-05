#pragma once
#ifndef _ASSET_ROUTING
#define _ASSET_ROUTING

#define ASSET_FOLDER "./WebPage/assets"
#define CSS_ASSET_ROUTE "/assets/css/"
#define JS_ASSET_ROUTE "/assets/js/"
#define FONT_ASSET_ROUTE "/assets/ttf/"
#define IMAGE_ASSET_ROUTE "/assets/images/"

typedef struct Assets
{
    size_t size;
    char **array;
    size_t folder_count;
    char **folders;
} Assets;

Assets getAssets();
void defineAssetRoutes();
void printFolderContents();
void cleanUp(Assets *assets);
void getAssetsFromFolder(const char *path, Assets *assets);
char *getAssetLocation(char *filename);
enum ASSET_TYPE getAssetType(char *filename);

#endif