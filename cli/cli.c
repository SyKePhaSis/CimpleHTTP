#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "create") == 0 && argc == 4)
    {
        if (strcmp(argv[2], "handler") == 0)
        {
            printf("[*] Creating handler with the name: %s", argv[3]);
            createHandler(argv[3]);
        }
    }
    else if (strcmp(argv[1], "serve") == 0)
    {
        system("app.exe");
    }
    else
    {
        printf("[*] Wrong Number of parameters.");
        exit(0);
    }
    return 0;
}

void createHandler(char *name)
{
    char *destination_path_h = malloc(strlen(name) + strlen("./src/Headers/Handlers/") + 3);
    destination_path_h[0] = 0x00;
    strcat(destination_path_h, "./src/Headers/Handlers/");
    strcat(destination_path_h, name);
    strcat(destination_path_h, ".h");
    char *destination_path_c = malloc(strlen(name) + strlen("./Handlers/") + 3);
    destination_path_c[0] = 0x00;
    strcat(destination_path_c, "./Handlers/");
    strcat(destination_path_c, name);
    strcat(destination_path_c, ".c");
    createFileFromTemplateHandler("./src/Templates/handler-c", destination_path_c, name);
    createFileFromTemplateHandler("./src/Templates/handler-h", destination_path_h, name);
}

void createFileFromTemplateHandler(char *path, char *destination, char *name)
{
    char *template = getTemplate(path);
    if (template == NULL)
    {
        printf("Couldn't open file, returned NULL");
    }
    char *output = findAndReplace(template, "#name#", name);
    FILE *fp = fopen(destination, "wb");
    if (fp == NULL)
    {
        return;
    }
    fwrite(output, 1, strlen(output), fp);
    fclose(fp);
    free(output);
    free(template);
}

char *findAndReplace(char *str, char *find, char *replace)
{
    char *occ = strstr(str, find); // Pointer to substring
    size_t size = 1;
    size_t index = 0;
    char *res = NULL;
    while (occ != NULL)
    {
        // occ - str = The Size of text to be copied from original string
        // strlen(replace) = Size of replacement string
        size += (size_t)(occ - str) - index + strlen(replace); // final size of char* res
        if (res == NULL)
        {
            res = malloc(size);
            res[0] = '\0';
        }
        else
        {
            res = realloc(res, size);
        }
        // Size of memory to copy occ - str = substring point - copy starting point
        strncat(res, str + index, (size_t)(occ - str) - index);
        // Point to start copying strin occurence + find
        index = (size_t)occ + strlen(find) - (size_t)str;
        // strncat(res, replace, strlen(replace));
        strcat(res, replace);
        occ = strstr(str + index, find);
    }
    res = realloc(res, size + strlen(str + index));
    strcat(res, str + index);
    return res;
}

char *getTemplate(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        return NULL;
    }
    long fsize = getFileLen(fp);
    char *lines = malloc(fsize);
    fread(lines, 1, fsize, fp);
    lines[fsize] = 0x00;
    fclose(fp);
    return lines;
}

long getFileLen(FILE *fp)
{
    long f_size;
    fseek(fp, 0L, SEEK_END);
    f_size = ftell(fp);
    rewind(fp);
    return f_size;
}