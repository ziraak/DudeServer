//
// Created by osboxes on 28/04/15.
//

#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#endif //DUDESERVER_USER_H

#include "database.h"



int checkUser(char *userName);

char **getUserList();

int getUser(char *username, userInfo *result);

void userJoinChannel(char *username, char *channelName);