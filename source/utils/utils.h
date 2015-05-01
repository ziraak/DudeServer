//
// Created by osboxes on 21/04/15.
//

#ifndef UTILS_H
#define UTILS_H

#include <glob.h>
#include <string.h>
#include <malloc.h>
#include "../main.h"

typedef struct commandStruct
{
    char *command;
    char *trailing;
    char **parameters;
    int parameterCount;
} commandStruct;

int substringCharacter(char *str, char **result);
int parseCommand(char *message, commandStruct *command);
void commandStruct_free(commandStruct* cmdStruct);

#endif