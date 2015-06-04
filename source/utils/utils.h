//
// Created by osboxes on 21/04/15.
//

#ifndef UTILS_H
#define UTILS_H

#define FREE(v) free(v); v = NULL

#include <glob.h>
#include <string.h>
#include <malloc.h>
#include "../main.h"

char* substringCharacter(char *str, int *result);

commandStruct commandStruct_initialize(char *message);
void commandStruct_free(commandStruct *cmdStruct);

void *MALLOC(size_t size);

#endif