//
// Created by osboxes on 21/04/15.
//
#ifndef DUDESERVER_JOIN_H
#define DUDESERVER_JOIN_H
#include "../main.h"
#include "../utils/utils.h"
#include "../medium.h"
#endif

int handleJoinCommand(char *message);

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey);

int joinChannel(channelInfo channel);

int createChannel(char *channelName, char *optionalChannelKey);
