//
// Created by osboxes on 21/04/15.
//

#include "../main.h"
#include "../utils/utils.h"
#include "../medium.h"

int handleJoinCommand(char *message);

int findChannelByName(char *channelName);

int authenticateChannel(char *channelName, char *optionalChannelKey);

int joinChannel(char *channelName);

int createChannel(char *channelName, char *optionalChannelKey);
