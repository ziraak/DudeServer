//
// Created by osboxes on 21/04/15.
//

#include "utils.h"

void toUppercase(char *str)
{
    while(*str != '\0')
    {
        *str = (char)toupper(*str);
        str++;
    }
}

int find(char *str, char find)
{
    timeStart;
    char* e = strchr(str, find);

    if(e == NULL)
    {
        return -1;
    }

    int index = (int)(e - str);
    timeEnd("utils/find");
    return index;
}

char* substringCharacter(char *str, int *result)
{
    timeStart;
    if(*str == ':' || *str == '\0')
    {
        *result = -1;
        return NULL;
    }


    size_t i = strcspn(str, " ");
    char* ret = MALLOC(i + 1);
    strncpy(ret, str, i);

    str += i;
    while(*str == ' ') { str++; i++; }


    *result = (int)i;
    timeEnd("utils/substringCharacter");
    return ret;

}

commandStruct commandStruct_initialize(char *message)
{
    timeStart;
    char *command = NULL,
         *trailing = NULL,
         **parameters = MALLOC(sizeof(char *));

    int offset;
    command = substringCharacter(message, &offset);
    toUppercase(command);
    message += offset;

    int parameterCount = 0;
    for(; ; parameterCount++)
    {
        REALLOC(parameters, sizeof(char *) * (parameterCount + 1));
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
        trailing = MALLOC(strlen(message) + 1);
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

    timeEnd("commandstruct_initialize");
    return cmd;
}

void commandStruct_free(commandStruct *cmdStruct)
{
    timeStart;
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
        timeEnd("commandstruct free");
    }
    timeEnd("commandstruct free");
}

void *_malloc(size_t size)
{
    timeStart;
    void* result = malloc(size);
    if(result == NULL)
    {
        perror("MALLOC failed.");
        exit(-1);
    }
    bzero(result, size);
    timeEnd("malloc");
    return result;
}


void *_realloc(void *r, size_t size)
{
    timeStart;
    void *result = realloc(r, size);
    if(result == NULL)
    {
        perror("REALLOC failed.");
        exit(-2);
    }
    timeEnd("realloc");
    return result;
}
