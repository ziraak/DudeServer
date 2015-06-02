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

int _innerGetChannel(sqlite3_stmt *stmt, channelInfo *channel);
channelInfo *_innerGetChannels(sqlite3_stmt *stmt, int *result);

channelInfo* getChannels(char* columns, int *result);
channelInfo* getVisibleChannels(char* columns, int *result);
int getChannelByName(char *channelName, channelInfo *channel);

int checkChannel(char *channelName);

int checkIfChannelEmpty(char* channelName);

messageInfo* getMessagesOnTime(char *channelName, int timestamp, int *result);

int insertChannel(char *channelName, char *password, char *topic, int visible);

int checkIfChannelHasPassword(char* channelname);

int deleteChannel(char *channelName);

int authenticateChannelPassword(char* channelname,char* password);

void updateChannelPassword(char *channelname, char *newPass);

void updateChannelTopic(char *channelname, char *newTopic);

int checkIfChannelVisible(char* channelName);

char*getChannelUserRole(char *channelName, char *username);

void updateChannelVisibility(char *channelName, int visible);

#endif 
