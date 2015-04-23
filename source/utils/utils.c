//
// Created by osboxes on 21/04/15.
//

#include <glob.h>
#include <string.h>
#include <malloc.h>
#include "utils.h"

int substringCharacter(char *str, char **result)
{
    char find = ' ';
    size_t i = strcspn(str, &find);
    *result = malloc(i);
    memcpy(*result, str, i);
    return (int) ++i;
}