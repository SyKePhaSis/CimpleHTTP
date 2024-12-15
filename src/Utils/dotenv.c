#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "Utils/dotenv.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include "Utils/fileHandling.h"

dotenv EnvFile = {.count = 0, .keys = NULL, .values = NULL};

/*
    File Specific Struct
*/

typedef struct deSEP
{
    char *st;
    char *end;
    size_t size;
} deSEP;

/* Memmory Management */

void freeEnv()
{
    for (unsigned int i = 0; i < EnvFile.count; i++)
    {
        mass_deallocation(2, EnvFile.keys[EnvFile.count], EnvFile.values[EnvFile.count]);
    }
    mass_deallocation(2, EnvFile.keys, EnvFile.values);
}

/*  HELPER FUNCTIONS  */

void printDotEnvDict(void)
{
    if (EnvFile.keys != NULL && EnvFile.values != NULL)
    {
        for (unsigned int i = 0; i < EnvFile.count; i++)
        {
            logInfo("%s - %s", EnvFile.keys[i], EnvFile.values[i]);
        }
    }
}

deSEP getQuotesPos(const char *str)
{
    deSEP r;
    char *ps = strchr(str, '"');
    if (ps != NULL)
    {
        char *pe = strrchr(str, '"');
        if (pe == NULL)
        {
            logError("Parsing Error while Reading .env File");
            r.size = 0;
            return r;
        }
        r.st = ps + 1;
        r.end = pe;
        r.size = (size_t)(r.end - r.st);
    }
    else
    {
        logError("Parsing Error while Reading .env File");
        r.size = 0;
    }
    return r;
}

/*   */

void parseDotEnv()
{
    FILE *fp = openDotEnv();
    if (fp != NULL)
    {
        long flen = getFileLen(fp);
        unsigned long c = 0;
        char *_buf = allocate(flen * sizeof(char) + 1);
        fread(_buf, 1, flen + 1, fp);

        char *tok = strtok(_buf, "\r\n");
        while (tok != NULL)
        {
            c++;
            char fc = getFirstChar(tok);
            if (fc != 0x00 && fc != '#')
            {
                char *epoint = strchr(tok, '=');
                if (epoint != NULL)
                {
                    size_t klength = epoint - tok;
                    if (EnvFile.keys == NULL || EnvFile.values == NULL)
                    {
                        EnvFile.keys = allocate(sizeof(char *));
                        EnvFile.values = allocate(sizeof(char *));
                    }
                    else
                    {
                        EnvFile.keys = reallocate(EnvFile.keys, (EnvFile.count + 1) * sizeof(char *));
                        EnvFile.values = reallocate(EnvFile.values, (EnvFile.count + 1) * sizeof(char *));
                    }
                    EnvFile.keys[EnvFile.count] = allocate(klength + 1);
                    strncpy(EnvFile.keys[EnvFile.count], tok, klength);
                    *(EnvFile.keys[EnvFile.count] + klength) = 0x00;

                    deSEP ds = getQuotesPos(tok);
                    if (ds.size == 0)
                    {
                        logError("Error Parsing .env File on line %d", c);
                        deallocate(EnvFile.keys[EnvFile.count]);
                        freeEnv();
                        return;
                    }

                    EnvFile.values[EnvFile.count] = allocate(ds.size + 1);
                    strncpy(EnvFile.values[EnvFile.count], ds.st, ds.size);
                    *(EnvFile.values[EnvFile.count] + ds.size) = 0x00;
                    EnvFile.count++;
                }
            }
            tok = strtok(NULL, "\r\n");
        }
        // sanitizeDotEnvDict();
        printDotEnvDict();
    }
    else
    {
        logError("Couldn't open the .env file");
    }
}

int keyExists(char *key)
{
    if (isDotEnvSet())
    {
        for (unsigned int i = 0; i < EnvFile.count; i++)
        {
            if (strcmp(key, EnvFile.keys[i]) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int isDotEnvSet()
{
    if (EnvFile.keys == NULL || EnvFile.values == NULL || EnvFile.count == 0)
    {
        return 0;
    }
    return 1;
}

char *getEnvValue(char *key)
{
    if (isDotEnvSet())
    {
        for (unsigned int i = 0; i < EnvFile.count; i++)
        {
            if (strcmp(key, EnvFile.keys[i]) == 0)
            {
                return EnvFile.values[i];
            }
        }
    }
    return NULL;
}

FILE *openDotEnv()
{
    FILE *fp;
    if (fileExists(".env"))
    {
        fp = fopen(".env", "r");
        return fp;
    }
    else
    {
        return NULL;
    }
}

dotenv *getDotEnv()
{
    return &EnvFile;
}

char getFirstChar(char *str)
{
    char fc = 0x00;
    size_t offset = 0;
    do
    {
        if (*(str + offset) != ' ')
        {
            fc = *(str + offset);
            break;
        }
        if (*(str + offset) == 0x00)
            break;
        offset++;
    } while (fc == 0x00);
    return fc;
}