//
// Created by osboxes on 02/06/15.
//

#ifndef DUDESERVER_CHANNELUSERS_H
#define DUDESERVER_CHANNELUSERS_H

#include "database.h"

channelInfo* getUserChannels(char *username, int *result);
void updateChannelUserRole(char *channelName, char *username, char *newRole);
int checkIfChannelEmpty(char *channelName);

#endif //DUDESERVER_CHANNELUSERS_H
