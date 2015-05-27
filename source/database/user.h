#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

int checkUser(char *userName);

char **getUserList();

int getUser(char *username, userInfo *result);

int userJoinChannel(char *username, char *channelName, char *userRole);

void deleteChannelFromUser(char *username, char *channelName);

int deleteUserFromList(char *username);

int deleteUser(char *username);

int changeNickname(char *username, char *newNickname);

int changePassword(char *username, char *newPassword);

int assignLoginToken(char *username, char *loginToken);

int createNewUser(char *username, char *password);

char* getUserNickname(char* username);

#endif
