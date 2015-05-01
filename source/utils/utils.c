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

    printf("%c on pos %i\n", find, offset);

    return (int)offset;
}

int substringCharacter(char *str, char **result)
{
    char find = ' ';
    size_t i = strcspn(str, &find);
    *result = malloc(i + 1);
    bzero(*result, i + 1);
    strncpy(*result, str, i);
    return (int) ++i;
}

int parseCommand(char *message, commandStruct *command)
{
    command->message = message;
    command->parameters = malloc(sizeof(message));
    char** parameters = command->parameters;
    int offset = substringCharacter(message, &command->command);

    while(find(message, ' ') < find(message, ':'))
    {
        char* result = NULL;
        offset = substringCharacter(message += offset, &result);

        printf("%s\n", result);

        if(offset < 1)
        {
            break;
        }

        *command->parameters = result;
        command->parameters++;
        free(result);
    }

    printf("TRAILING\n");

    return BOOL_TRUE;
}