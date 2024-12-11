#include "Core/Routes.h"
#include "Core/assetRouting.h"
#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include "Utils/dotenv.h"
#include "Utils/dynamicArray.h"

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <winnt.h>

Assets *getAssets()
{
    Assets *assetsFL = allocate(sizeof(Assets));
    assetsFL->assets = getArray(CHAR_ARR);
    assetsFL->folders = getArray(CHAR_ARR);
    char *path = NULL;
    getAssetsFromFolder(getEnvValue("ASSET_FOLDER"), assetsFL);
    for (long i = 0; i < assetsFL->folders->size; i++)
    {
        size_t allocation_size = strlen(getEnvValue("ASSET_FOLDER")) + strlen(getFromArray(assetsFL->folders, i)) + 2;
        path = allocate(allocation_size);
        StringCchCopy(path, allocation_size, getEnvValue("ASSET_FOLDER"));
        StringCchCat(path, allocation_size, "/");
        StringCchCat(path, allocation_size, getFromArray(assetsFL->folders, i));
        getAssetsFromFolder(path, assetsFL);
        deallocate(path);
    }
    return assetsFL;
}

void getAssetsFromFolder(const char *path, Assets *assets)
{
    WIN32_FIND_DATA ffd;
    TCHAR *szDir = allocate(sizeof(PTCHAR) * (strlen(path) + 3));
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    StringCchCopy(szDir, (strlen(path) + 3), path);
    StringCchCat(szDir, (strlen(path) + 3), TEXT("/*"));
    logSuccess("Path: %s", szDir);

    hFind = FindFirstFile(szDir, &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        logError("Couldn't List the Directory");
        deallocate(szDir);
    }

    logInfo("------ %s ASSETS DIRECTORY ------", path);
    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            logInfo("<DIR>   %s  ", ffd.cFileName);
            if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0)
            {
                addToArray(assets->folders, ffd.cFileName);
                logSuccess("Folder added %s", ffd.cFileName);
            }
        }
        else
        {
            addToArray(assets->assets, ffd.cFileName);
            logInfo("<FILE>  %s  ", ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        logError("Error occured: %d", dwError);
    }
    FindClose(hFind);
    deallocate(szDir);
}

void defineAssetRoutes()
{
    Assets *assets = getAssets();
    size_t size;
    for (long i = 0; i < assets->assets->size; i++)
    {
        logInfo("Routing asset '%s'", getFromArray(assets->assets, i));
        char *location = getAssetLocation(getFromArray(assets->assets, i));
        if (location == NULL)
        {
            logInfo("Asset '%s' skipped for routing", getFromArray(assets->assets, i));
            continue;
        }
        size = strlen(getFromArray(assets->assets, i)) + strlen(location) + 1;
        char *path = allocate(size);
        StringCchCopyA(path, size, location);
        StringCchCatA(path, size, getFromArray(assets->assets, i));
        enum ASSET_TYPE type = getAssetType(getFromArray(assets->assets, i));
        defineAssetRoute(path, type);
    }
    cleanUp(assets);
}

char *getAssetLocation(char *filename)
{
    char *ext = getFileExt(filename);
    if (strcmp(ext, "css") == 0)
    {
        return CSS_ASSET_ROUTE;
    }
    else if (strcmp(ext, "js") == 0)
    {
        return JS_ASSET_ROUTE;
    }
    else if (strcmp(ext, "ttf") == 0 || strcmp(ext, "woff") == 0 || strcmp(ext, "woff2") == 0)
    {
        return FONT_ASSET_ROUTE;
    }
    else if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
    {
        return IMAGE_ASSET_ROUTE;
    }
    return NULL;
}

enum ASSET_TYPE getAssetType(char *filename)
{
    char *ext = getFileExt(filename);
    if (strcmp(ext, "css") == 0)
    {
        return CSS;
    }
    else if (strcmp(ext, "js") == 0)
    {
        return JS;
    }
    else if (strcmp(ext, "ttf") == 0 || strcmp(ext, "woff") == 0 || strcmp(ext, "woff2") == 0)
    {
        return FONT;
    }
    else if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
    {
        return IMAGE;
    }
}

// HELP

void printFolderContents()
{
    Assets *assets = getAssets();
    logSuccess("Assets Size: %lu", assets->assets->size);
    printArray(assets->assets);
    cleanUp(assets);
    return;
}

void cleanUp(Assets *assets)
{
    if (assets != NULL)
    {
        freeArray(assets->folders);
        freeArray(assets->assets);
    }
    else
    {
        logError("Assets are NULL");
        return;
    }
}
