#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

int checkIfUserExists(char *username);

int getUser(char *username, userInfo *result);

int isUserInChannel(char* channelname, char* username);

int userJoinChannel(char *username, char *channelName, char *userRole);

int deleteUser(char *username);

int changeNickname(char *username, char *newNickname);

int changePassword(char *username, char *newPassword);

int createNewUser(char *username, char *password);

char* getUserNickname(char* username);

int userLeaveChannel(char* username, char *channelname);

#endif
