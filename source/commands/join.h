//
// Created by osboxes on 21/04/15.
//

#ifndef DUDESERVER_JOIN_H
#define DUDESERVER_JOIN_H

#endif //DUDESERVER_JOIN_H

int handleJoinCommand(char *message);

int findChannelByName(char *channelName);
int authenticateChannel(char *channelName, char *optionalChannelKey);
int joinChannel(char *channelName);
int createChannel(char *channelName, char *optionalChannelKey);
