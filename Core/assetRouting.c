#include "Core/Routes.h"
#include "Core/assetRouting.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>

Assets getAssets()
{
    WIN32_FIND_DATA ffd;
    TCHAR *szDir = allocate(sizeof(PTCHAR) * (strlen(CSS_ASSET_FOLDER) + 3));
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    Assets assetsFL;
    assetsFL.size = 0;
    assetsFL.array = NULL;

    StringCchCopy(szDir, (strlen(CSS_ASSET_FOLDER) + 3), TEXT(CSS_ASSET_FOLDER));
    StringCchCat(szDir, (strlen(CSS_ASSET_FOLDER) + 3), TEXT("/*"));
    logSuccess("Path: %s", szDir);

    hFind = FindFirstFile(szDir, &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        logError("Couldn't List the Directory");
        deallocate(szDir);
        assetsFL.array = NULL;
        return assetsFL;
    }

    logInfo("------ ASSETS DIRECTORY ------");
    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            logInfo("<DIR>   %s  ", ffd.cFileName);
        }
        else
        {
            assetsFL.size++;
            if (assetsFL.size == 1)
            {
                assetsFL.array = allocate(sizeof(char *) * (assetsFL.size));
            }
            else
            {
                assetsFL.array = reallocate(assetsFL.array, sizeof(char *) * (assetsFL.size));
            }
            assetsFL.array[assetsFL.size - 1] = allocate(sizeof(char) * (strlen(ffd.cFileName) + 1));
            StringCchCopyA(assetsFL.array[assetsFL.size - 1], strlen(ffd.cFileName) + 1, ffd.cFileName);
            logInfo("<FILE>  %s  ", ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    if (assetsFL.size == 1)
    {
        deallocate(assetsFL.array);
        assetsFL.array = NULL;
    }

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        logError("Error occured: %d", dwError);
    }
    FindClose(hFind);
    deallocate(szDir);
    return assetsFL;
}

void defineAssetRoutes()
{
    Assets assets = getAssets();
    size_t size;

    if (assets.array != NULL && assets.array[assets.size - 1] != NULL)
    {
        for (size_t i = 0; i < assets.size; i++)
        {
            size = strlen(assets.array[i]) + strlen(CSS_ASSET_ROUTE) + 1;
            char *path = allocate(size);
            StringCchCopyA(path, size, CSS_ASSET_ROUTE);
            StringCchCatA(path, size, assets.array[i]);
            defineAssetRoute(path);
        }
    }
    else
    {
        logError("Couldn't read assets array");
    }
    cleanUp(&assets);
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
    }
    else
    {
        logError("Assets are NULL");
        return;
    }
}
