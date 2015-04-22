//
// Created by osboxes on 20/04/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <malloc.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>


typedef struct userinfo {
    char *username;
    char *nickname;
    char *password;
    char **channels;
} userinfo;

/**
 * main for the medium
 */
void mainMedium();

userinfo getUser(char *username);

