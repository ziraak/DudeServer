//
// Created by desmond on 4/24/15.
//

#ifndef DUDESERVER_CHANNEL_H
#define DUDESERVER_CHANNEL_H

#include "database.h"
#include <libxml/xmlstring.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <stdio.h>
#include <stdlib.h>


static char *const FILEFORMATSTRING = "xml/channels/%s.xml";
static xmlChar *const channelTagName = "channel";
static xmlChar *const nameTagName = "name";
static const int maxFilenameSize = 250;
static const int compression = 0;
static xmlChar *const usersTagName = "users";
static xmlChar *const userTagName = "user";
static xmlChar *const messagesTagName = "messages";
static xmlChar *const messageTagName = "message";
static xmlChar *const timestampTagName = "timestamp";
static xmlChar *const bodyTagName = "body";
static const int maxMessages = 100;

channelInfo* getChannels(char* columns, int *result);
int getChannelByName(char *channelName, channelInfo *channel);

int checkChannel(char *channelName);

int checkIfChannelEmpty(char* channelName);

messageInfo* getMessagesOnTime(char *channelName, int timestamp);

void createNewChannel(char *channelName, char *password, char *topic, int visible);

int checkIfChannelHasPassword(char* channelname);

int deleteChannelInDB(char *channelName);

int authenticateChannelPassword(char* channelname,char* password);

void newChannelPassword(char* channelname, char* newPass);

void newChannelTopic(char* channelname,char* newTopic);

int checkIfChannelVisible(char* channelName);

channelUser* getUsersFromChannel(char *channelName);

char* getUserRole(char* channelName, char* username);

void setChannelVisibility(char* channelname,int visible);

void setChannelUserRole(char* channelname, char* username, char* newRole);


#endif 
