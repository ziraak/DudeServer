#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

int checkUser(char *userName);
char **getUserList();
int getUser(char *username, userInfo *result);
void userJoinChannel(char *username, char *channelName);

#endif
