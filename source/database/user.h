#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

int checkUser(char *userName);

char **getUserList();

int getUser(char *username, userInfo *result);

void userJoinChannel(char *username, char *channelName);

void deleteChannelFromUser(char *username, char *channelName);

void deleteUserFromList(char *username);

void deleteUser(char *username);

#endif
