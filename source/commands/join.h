#ifndef DUDESERVER_JOIN_H
#define DUDESERVER_JOIN_H
#include "../main.h"
#include "../utils/utils.h"

int handleJoinCommand(char *message);

int findChannelByName(char *channelName);

int authenticateChannel(char *channelName, char *optionalChannelKey);

int joinChannel(char *channelName);

int createChannel(char *channelName, char *optionalChannelKey);
