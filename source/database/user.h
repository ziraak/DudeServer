#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

int checkUser(char *userName);

char **getUserList();

int getUser(char *username, userInfo *result);

int userJoinChannel(char *username, char *channelName);

void deleteChannelFromUser(char *username, char *channelName);

void deleteUserFromList(char *username);

void deleteUser(char *username);

void changeNickname(char * username, char * newNickname);

void changePassword(char* username, char* newPassword);

void assignLoginToken(char* username, char* loginToken);

void createNewUser(char* username, char* password);

#endif
