#ifndef DUDESERVER_JOIN_H
#define DUDESERVER_JOIN_H
#include "../main.h"
#include "../utils/utils.h"
#include "../database/database.h"

int handleJoinCommand(char *message);

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey);

int joinChannel(char* channelName);

int createChannel(char *channelName, char *optionalChannelKey);

int userHasChannel(char* channelName);

void userAddChannel(char* channelName);

#endif
