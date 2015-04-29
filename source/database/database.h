#ifndef DUDESERVER_DATABASE_H
#define DUDESERVER_DATABASE_H

#include "databaseStructs.h"
#include "channel.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <malloc.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>


void mainMedium();

xmlDocPtr openDoc(char *docname);

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType);

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname);

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname);

void deleteField(xmlDocPtr doc, xmlNodePtr currentNode, char *fieldText);

void addChild(xmlNodePtr cur, char *parent, char *child, char *childContent);

void addFieldToFileInList(char *fileType, char *filename, char *listname, char *fieldname, char *content);

#endif
