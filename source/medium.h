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

typedef struct userInfo
{
    char *username;
    char *nickname;
    char *password;
    char **channels;
    char *loginToken;
} userInfo;

typedef struct messageInfo
{
    char *writer;
    char *timestamp;
    char *body;
} messageInfo;

typedef struct channelInfo
{
    char *name;
    char **users;
    messageInfo *messages;
} channelInfo;

int getUser(char *username, userInfo* result);

int getChannel(char *channelName, channelInfo* channel);

int checkChannel(char *channelName);

int checkUser(char *userName);

char **getUserList();

char **getChannelList();

xmlDocPtr openDoc(char *docname);

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType);

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname);

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname);
