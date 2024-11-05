#include "Core/Routes.h"
#include "Core/assetRouting.h"
#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <winnt.h>

Assets getAssets()
{
    Assets assetsFL = {
        .array = NULL,
        .folder_count = 0,
        .folders = allocate(sizeof(char *) * 5),
        .size = 0};
    char *path = NULL;
    getAssetsFromFolder(ASSET_FOLDER, &assetsFL);
    for (size_t i = 0; i < assetsFL.folder_count; i++)
    {
        size_t allocation_size = strlen(ASSET_FOLDER) + strlen(assetsFL.folders[i]) + 2;
        path = allocate(allocation_size);
        StringCchCopy(path, allocation_size, ASSET_FOLDER);
        StringCchCat(path, allocation_size, "/");
        StringCchCat(path, allocation_size, assetsFL.folders[i]);
        getAssetsFromFolder(path, &assetsFL);
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
                assets->folders[assets->folder_count] = allocate(strlen(ffd.cFileName) + 1);
                StringCchCopyA(assets->folders[assets->folder_count], strlen(ffd.cFileName) + 1, ffd.cFileName);
                assets->folder_count++;
                logSuccess("Folder added %s", ffd.cFileName);
            }
        }
        else
        {
            assets->size++;
            if (assets->size == 1)
            {
                assets->array = allocate(sizeof(char *) * (assets->size));
            }
            else
            {
                assets->array = reallocate(assets->array, sizeof(char *) * (assets->size));
            }
            assets->array[assets->size - 1] = allocate(sizeof(char) * (strlen(ffd.cFileName) + 1));
            StringCchCopyA(assets->array[assets->size - 1], strlen(ffd.cFileName) + 1, ffd.cFileName);
            logInfo("<FILE>  %s  ", ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    if (assets->size == 1)
    {
        deallocate(assets->array);
        assets->array = NULL;
    }

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
    Assets assets = getAssets();
    size_t size;

    if (assets.array != NULL && assets.array[assets.size - 1] != NULL)
    {
        for (size_t i = 0; i < assets.size; i++)
        {
            logInfo("Routing asset '%s'", assets.array[i]);
            char *location = getAssetLocation(assets.array[i]);
            if (location == NULL)
            {
                logInfo("Asset '%s' skipped for routing", assets.array[i]);
                continue;
            }
            size = strlen(assets.array[i]) + strlen(location) + 1;
            char *path = allocate(size);
            StringCchCopyA(path, size, location);
            StringCchCatA(path, size, assets.array[i]);
            enum ASSET_TYPE type = getAssetType(assets.array[i]);
            defineAssetRoute(path, type);
        }
    }
    else
    {
        logError("Couldn't read assets array");
    }
    cleanUp(&assets);
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
    Assets assets = getAssets();
    logSuccess("Assets Size: %lu", assets.size);
    if (assets.array != NULL && assets.array[assets.size - 1] != NULL)
    {
        for (size_t i = 0; i < assets.size; i++)
        {
            logInfo("File: %s", assets.array[i]);
        }
    }
    else
    {
        logError("Couldn't read assets array");
    }
    cleanUp(&assets);
    return;
}

void cleanUp(Assets *assets)
{
    if (assets != NULL)
    {
        for (size_t i = 0; i < assets->size; i++)
        {
            deallocate(assets->array[i]);
        }
        deallocate(assets->array);
        for (size_t i = 0; i < assets->folder_count; i++)
        {
            deallocate(assets->folders[i]);
        }
        deallocate(assets->folders);
    }
    else
    {
        logError("Assets are NULL");
        return;
    }
}
