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

int substringCharacter(char *str, char **result)
{
    if(*str == ':' || *str == '\0')
    {
        return -1;
    }

    size_t i = strcspn(str, " ");
    *result = malloc(i + 1);
    bzero(*result, i + 1);
    strncpy(*result, str, i);

    str += i;
    while(*str == ' ') { str++; i++; }

    return (int) i;
}

commandStruct commandStruct_initialize(char *message)
{
    char *command = NULL,
         *trailing = NULL,
         **parameters = NULL;
    parameters = realloc(parameters, strlen(message));

    int offset = substringCharacter(message, &command);
    message += offset;

    int parameterCount = 0;
    for(; ; parameterCount++)
    {
        int off = find(message, ' '),
                semicolon = find(message, ':');

        if((semicolon != -1 && semicolon < off) && off > 1)
        {
            break;
        }

        off = substringCharacter(message, &parameters[parameterCount]);
        if(off < 0)
        {
            break;
        }
        message += off;
    }

    if(find(message, ':') != -1)
    {
        trailing = malloc(strlen(message) + 1);
        bzero(trailing, strlen(message) + 1);
        strncpy(trailing, ++message, strlen(message));
    }
    else
    {
        trailing = NULL;
    }

    commandStruct cmd = { .parameterCount = parameterCount, .command = command, .trailing = trailing, .parameters = parameters };

    return cmd;
}

void commandStruct_free(commandStruct *cmdStruct)
{
    if(cmdStruct == NULL)
    {
        return;
    }

    if(cmdStruct->parameters != NULL)
    {
        int j = 0;
        while(cmdStruct->parameters[j] != NULL && j < cmdStruct->parameterCount)
        {
            free(cmdStruct->parameters[j]);
            j++;
        }
    }

    if(cmdStruct->command != NULL)
    {
        free(cmdStruct->command);
    }

    if(cmdStruct->trailing != NULL)
    {
        free(cmdStruct->trailing);
    }
}