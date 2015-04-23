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


typedef struct userInfo {
    char *username;
    char *nickname;
    char *password;
    char **channels;
} userInfo;

typedef struct messageInfo {
    char *writer;
    char *timestamp;
    char *body;
}messageInfo;

typedef struct channelInfo{
    char *naam;
    char **users;
    messageInfo messages[100];
}channelInfo;



/**
 * main for the medium
 */
void mainMedium();

userInfo getUser(char *username);

