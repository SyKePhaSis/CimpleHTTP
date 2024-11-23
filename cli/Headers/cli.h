#ifndef CLI
#define CLI

#include <stdio.h>

void createHandler(char *name);
char *findAndReplace(char *str, char *find, char *replace);
char *getTemplate(char *filename);
long getFileLen(FILE *fp);
void createFileFromTemplateHandler(char *path, char *destination, char *name);

#endif
