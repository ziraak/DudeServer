//
// Created by osboxes on 21/04/15.
//

#include "utils.h"

int find(char *str, char find)
{
    char* e = strchr(str, find);

    if(e == NULL)
    {
        return -1;
    }

    int index = (int)(e - str);
    return index;
}

char* substringCharacter(char *str, int *result)
{
    if(*str == ':' || *str == '\0')
    {
        *result = -1;
        return NULL;
    }

    char* ret;

    size_t i = strcspn(str, " ");
    MALLOC(ret, i + 1);
    strncpy(ret, str, i);

    str += i;
    while(*str == ' ') { str++; i++; }

    *result = (int)i;
    return ret;
}

commandStruct commandStruct_initialize(char *message)
{
    char *command = NULL,
         *trailing = NULL,
         **parameters = NULL;
    MALLOC(parameters, sizeof(char *));

    int offset;
    command = substringCharacter(message, &offset);
    message += offset;

    int parameterCount = 0;
    for(; ; parameterCount++)
    {
        int off = find(message, ' '), semicolon = find(message, ':');

        if((semicolon != -1 && semicolon < off) && off > 1)
        {
            break;
        }

        char* c = substringCharacter(message, &off);
        if(off < 0 || c == NULL)
        {
            FREE(c);
            break;
        }
        parameters[parameterCount] = c;
        message += off;
    }

    if(find(message, ':') != -1)
    {
        MALLOC(trailing, strlen(message) + 1);
        strncpy(trailing, ++message, strlen(message));
    }
    else
    {
        trailing = NULL;
    }

//    commandStruct cmd = { .parameterCount = parameterCount, .command = command, .trailing = trailing, .parameters = parameters };
    commandStruct cmd;
    bzero(&cmd, sizeof(commandStruct));
    cmd.command = command;
    cmd.trailing = trailing;
    cmd.parameterCount = parameterCount;
    cmd.parameters = parameters;

    return cmd;
}

void commandStruct_free(commandStruct *cmdStruct)
{
    if(cmdStruct != NULL)
    {
        int j;
        for(j = 0; j < cmdStruct->parameterCount; j++)
        {
            FREE(cmdStruct->parameters[j]);
        }

        FREE(cmdStruct->parameters);
        FREE(cmdStruct->command);
        FREE(cmdStruct->trailing);
    }
}