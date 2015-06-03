//
// Created by osboxes on 02/06/15.
//

#ifndef DUDESERVER_CHANNELUSERS_H
#define DUDESERVER_CHANNELUSERS_H

#include "database.h"

channelInfo* getUserChannels(char *username, int *result);
userInfo* getChannelUsers(char *channelName, int *result);
int getChannelUser(char *channelName, char *username, channelUser *cu);

int isUserInChannel(char *channelName, char *username);

char *getChannelUserRole(char *channelName, char *username);
int updateChannelUserRole(char *channelName, char *username, char *newRole);
int checkIfChannelEmpty(char *channelName);
int userIsOperatorInChannel(char *channelName, char *username);

#endif //DUDESERVER_CHANNELUSERS_H
