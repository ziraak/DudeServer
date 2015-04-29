//
// Created by osboxes on 28/04/15.
//

#ifndef DUDESERVER_CHANNEL_H
#include "channel.h"
#endif

#ifndef DUDESERVER_USER_H
#include "user.h"
#endif //DUDESERVER_USER_H

#ifndef DUDESERVER_DATABASE_H
#define DUDESERVER_DATABASE_H



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


void mainMedium();


xmlDocPtr openDoc(char *docname);

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType);

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname);

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname);

void deleteField(xmlDocPtr doc, xmlNodePtr currentNode, char *fieldText);

void addChild(xmlNodePtr cur, char *parent, char *child, char *childContent);

void addFieldToFileInList(char *fileType, char *filename, char *listname, char *fieldname, char *content);

#endif //DUDESERVER_DATABASE_H