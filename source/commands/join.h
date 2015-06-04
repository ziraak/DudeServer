#ifndef DUDESERVER_JOIN_H
#define DUDESERVER_JOIN_H
#include "../main.h"
#include "../utils/utils.h"
#include "../database/database.h"

int handleJoinCommand(commandStruct cmd);

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey);

int joinChannel(char* channelName);

int joinChannelByUsername(char* channelName, char *username);

#endif
