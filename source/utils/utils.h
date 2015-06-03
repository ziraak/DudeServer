//
// Created by osboxes on 21/04/15.
//

#ifndef UTILS_H
#define UTILS_H

#define FREE(v) free(v); v = NULL
#define MALLOC(v, size) v = malloc(size); if(v == NULL) { exit(-1); } bzero(v, size)

#include <glob.h>
#include <string.h>
#include <malloc.h>
#include "../main.h"

char* substringCharacter(char *str, int *result);

commandStruct commandStruct_initialize(char *message);
void commandStruct_free(commandStruct *cmdStruct);

#endif