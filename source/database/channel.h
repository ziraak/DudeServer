//
// Created by desmond on 4/24/15.
//

#ifndef DUDESERVER_CHANNEL_H
#define DUDESERVER_CHANNEL_H

#include "database.h"
#include <stdio.h>
#include <stdlib.h>

int _innerGetChannel(sqlite3_stmt *stmt, channelInfo *channel);
channelInfo *_innerGetChannels(sqlite3_stmt *stmt, int *result);

channelInfo* getChannels(char* columns, int *result);
channelInfo* getVisibleChannels(char* columns, int *result);
int getChannelByName(char *channelName, channelInfo *channel);

int checkChannel(char *channelName);

int checkIfChannelEmpty(char* channelName);

messageInfo *getMessagesOnTime(char *channelName, time_t timestamp, int *result, int amountOfMessages);

int insertChannel(char *channelName, char *password, char *topic, int visible, int inviteOnly, int topicOperatorOnly);

int checkIfChannelHasPassword(char*channelName);

int deleteChannel(char *channelName);

int authenticateChannelPassword(char*channelName,char* password);

int checkIfChannelVisible(char* channelName);
int checkIfChannelInviteOnly(char *channelName);
int checkIfChannelTopicOperatorOnly(char *channelName);

char*getChannelUserRole(char *channelName, char *username);

void updateChannelTopic(char *channelname, char *newTopic);
void updateChannelPassword(char *channelname, char *newPass);
void updateChannelVisibility(char *channelName, int visible);
void updateChannelInviteOnly(char *channelName, int inviteOnly);
void updateChannelTopicOperatorOnly(char *channelName, int topicOperatorOnly);
void sendSystemMessageToChannel(char *msgToSend, char *channelName);

#endif 
