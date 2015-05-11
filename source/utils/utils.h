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
    char **parameters;
    char *trailing;
} commandStruct;

int substringCharacter(char *str, char **result);
int parseCommand(char *message, commandStruct *command);

#endif