#ifndef DUDESERVER_SERVER_H
#define DUDESERVER_SERVER_H

#include "main.h"
#include "ctype.h"
#include "errors.h"

#define INNER_BUFFER_LENGTH 1024
#define PRINT_ALL BOOL_FALSE

typedef struct _user
{
    int active;
    int authorized;
    int write;
    userInfo user;
} User;

time_t lastTimestamp;
userInfo currentUser;

void runServer(int port);
int commandEquals(commandStruct cmd, char *check);

User* getClient(int client);

void sendToClient(int client, char *message);
void sendToAllClients(char *message);

#endif //DUDESERVER_SERVER_H
