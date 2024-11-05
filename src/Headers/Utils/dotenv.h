#ifndef _ENV_PARSER
#define _ENV_PARSER

#include <stdio.h>

typedef struct dotenv
{
    char **keys;
    char **values;
    unsigned int count;
} dotenv;

int keyExists(char *key);
int isDotEnvSet();
void parseDotEnv();
FILE *openDotEnv();
dotenv *getDotEnv();
char *getEnvValue(char *key);
char getFirstChar(char *str);

#endif