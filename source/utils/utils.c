//
// Created by osboxes on 21/04/15.
//

#include "utils.h"

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
    int offset = substringCharacter(message, &command->command);

    return BOOL_TRUE;
}