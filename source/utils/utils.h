//
// Created by osboxes on 21/04/15.
//

#ifndef UTILS_H
#define UTILS_H

#define MALLOC(size) _malloc(size)
#define REALLOC(v, size) v = _realloc(v, size)
#define FREE(v) free(v); v = NULL

#include <glob.h>
#include <string.h>
#include <malloc.h>
#include "../main.h"

char* substringCharacter(char *str, int *result);

commandStruct commandStruct_initialize(char *message);
void commandStruct_free(commandStruct *cmdStruct);

void *_malloc(size_t size);
void *_realloc(void *r, size_t size);

#endif