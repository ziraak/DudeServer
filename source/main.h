#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/commandStruct.h"
#include "utils/utils.h"
#include "commands/login.h"
#include "commands/join.h"
#include "commands/privmsg.h"
#include "commands/create_user.h"
#include "commands/delete_user.h"
#include "commands/update_password.h"
#include "commands/update_nickname.h"
#include "database/database.h"
#include "commands/part.h"
#include "protocolDefines.h"
#include "time.h"

userInfo currentUser;

void runServer();

void exitIfError(ssize_t variableToCheckForError, char *errorMessage);

void processConnectedClient(int sockfd, struct sockaddr_in adres_client);

void processConnectedClientWithFork(int sockfd, struct sockaddr_in adres_client);

int setupServer();

void flushStdout();

void sendIntegerMessageToClient(int sockfd, int msg);

void sendMessageToClient(int sockfd, char *buffer, size_t bufferLength);

int parseMessage(char *message);

void acknowledgeConnection(int sockfd);

int commandEquals(char *command, char *check);

int authenticateClient(int sockfd, commandStruct cmd);

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#define MINIMUM_PASSWORD_LENGTH 6

#endif