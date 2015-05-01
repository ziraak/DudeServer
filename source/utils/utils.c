//
// Created by osboxes on 21/04/15.
//

#include "utils.h"

int find(char *str, char find)
{
    size_t offset = strcspn(str, &find);

    if(offset == strlen(str))
    {
        return -1;
    }

    return (int)offset;
}

int substringCharacter(char *str, char **result)
{
    if(*str == ':' || *str == '\0')
    {
        return -1;
    }

    char find = ' ';
    size_t i = strcspn(str, &find);
    *result = malloc(i + 1);
    bzero(*result, i + 1);
    strncpy(*result, str, i);

    str += i;
    while(*str == find) { str++; i++; }

    return (int) i;
}

int parseCommand(char *message, commandStruct *command)
{
    size_t m_size = sizeof(message);

    char** parameters = NULL;
    parameters = realloc(parameters, m_size);
    int offset = substringCharacter(message, &command->command);
    message += offset;

    int i = 0;
    for(; ; i++)
    {
        int off = find(message, ' '),
            semicolon = find(message, ':');

        if((semicolon != -1 && semicolon < off) && off > 1)
        {
            break;
        }

        off = substringCharacter(message, &parameters[i]);
        if(off < 0)
        {
            break;
        }
        message += off;
    }

    command->parameterCount = i;

    if(find(message, ':') != -1)
    {
        command->trailing = malloc(strlen(message) + 1);
        bzero(command->trailing, strlen(message) + 1);
        strncpy(command->trailing, ++message, strlen(message));
    }
    else
    {
        command->trailing = NULL;
    }

    command->parameters = parameters;

    return BOOL_TRUE;
}

void commandStruct_free(commandStruct* cmdStruct)
{
    if(cmdStruct == NULL) return;
    if(cmdStruct->parameters != NULL) free(cmdStruct->parameters);
    if(cmdStruct->command != NULL) free(cmdStruct->command);
    if(cmdStruct->trailing != NULL) free(cmdStruct->trailing);
}