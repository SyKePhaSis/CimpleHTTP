
#include <windows.h>
#include <WinBase.h>

#include "Core/fileWatcher.h"
#include "Utils/memmory.h"
#include "Utils/dotenv.h"
#include "Utils/logger.h"

/*

    HELPERS

*/

char *WideCharToCharP(WCHAR *wc)
{
    size_t sizeRequired = WideCharToMultiByte(CP_ACP, 0, wc, -1,
                                              NULL, 0, NULL, NULL);
    char *buf = allocate(sizeRequired + 1);
    WideCharToMultiByte(CP_ACP, 0, wc, -1, buf, sizeRequired, NULL, NULL);
    return buf;
}

/*

    EXPORTED FUNCTIONS

*/

WatcherObject *getDirectoryWatcher(char *path)
{
    WatcherObject *wo = allocate(sizeof(WatcherObject));
    wo->hDir = CreateFile(TEXT(path), FILE_LIST_DIRECTORY,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          NULL, OPEN_EXISTING,
                          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                          NULL);
    if (wo->hDir == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        logError("File watcher couldn't find directory to watch %s because it doesn't exist.", path);
        return NULL;
    }
    else if (wo->hDir == INVALID_HANDLE_VALUE)
    {
        logError("Couldn't get handle for directory watcher, aborting...");
        return NULL;
    }

    wo->overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

    wo->buffsize = 1024;
    wo->buff = allocate(sizeof(unsigned char) * wo->buffsize);
    int success = ReadDirectoryChangesW(
        wo->hDir, wo->buff, wo->buffsize, TRUE,
        FILE_NOTIFY_CHANGE_LAST_WRITE,
        NULL, &(wo->overlapped), NULL);
    if (success > 0)
    {
        logSuccess("File watcher successfuly created");
        return wo;
    }
    return NULL;
}

retCheckWatcher checkForUpdate(WatcherObject *wo)
{
    DWORD result = WaitForSingleObject(wo->overlapped.hEvent, 0);
    retCheckWatcher ret;
    ret.updated = 0;
    ret.fileName = NULL;
    if (result == WAIT_OBJECT_0)
    {
        DWORD bytes_transferred;
        GetOverlappedResult(wo->hDir, &(wo->overlapped), &bytes_transferred, FALSE);
        FILE_NOTIFY_INFORMATION *event = (FILE_NOTIFY_INFORMATION *)wo->buff;

        for (;;)
        {
            switch (event->Action)
            {
            case FILE_ACTION_MODIFIED:
                ret.fileName = WideCharToCharP(event->FileName);
                break;

            default:
                printf("Other Case");
                break;
            }

            if (event->NextEntryOffset)
            {
                *((UINT8 **)&event) += event->NextEntryOffset;
            }
            else
            {
                break;
            }
        }

        int success = ReadDirectoryChangesW(
            wo->hDir, wo->buff, wo->buffsize, TRUE,
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            NULL, &(wo->overlapped), NULL);

        if (success)
            ret.updated = 1;
        return ret;
    }
    return ret;
}

void RefreshDirectory()
{
}