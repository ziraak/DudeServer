#ifndef DUDESERVER_SERVER_H
#define DUDESERVER_SERVER_H

#include "main.h"

int lastTimestamp;
userInfo currentUser;

void freeCurrentUser();

void runServer(int USE_FORK, int port);

void exitIfError(ssize_t variableToCheckForError, char *errorMessage);

void processConnectedClient();

void processConnectedClientWithFork();

void flushStdout();

int parseMessage(char *message);

int commandEquals(commandStruct cmd, char *check);

int authenticateClient(commandStruct cmd);

#endif //DUDESERVER_SERVER_H
