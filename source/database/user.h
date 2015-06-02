#ifndef DUDESERVER_USER_H
#define DUDESERVER_USER_H

#include "database.h"

void fillUser(sqlite3_stmt *statement, userInfo *user);
userInfo *_innerGetUsers(sqlite3_stmt *stmt, int *result);

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

void user_free(userInfo *user);
void users_free(userInfo *users, int amount);

#endif
