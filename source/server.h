#ifndef DUDESERVER_SERVER_H
#define DUDESERVER_SERVER_H

#include "main.h"

userInfo currentUser;
struct timespec sendWait;

void runServer(int USE_FORK, int port);

void exitIfError(ssize_t variableToCheckForError, char *errorMessage);

void processConnectedClient(int sockfd, struct sockaddr_in adres_client);

void processConnectedClientWithFork(int sockfd, struct sockaddr_in adres_client);

int setupServer(int port);

void flushStdout();

int parseMessage(char *message, int sockfd);

void acknowledgeConnection(int sockfd);

int commandEquals(commandStruct cmd, char *check);

int authenticateClient(int sockfd, commandStruct cmd);

#endif //DUDESERVER_SERVER_H
