#ifndef DUDESERVER_MESSAGES_H
#define DUDESERVER_MESSAGES_H

#include "../main.h"
#include "../database/databaseStructs.h"
#include "../database/databaseStructs_free.h"

typedef struct channelMessagesStruct
{
    char* channelName;
    char** messages;
    int messageCount;
} channelMessagesStruct;

typedef struct pollStruct
{
    int timestamp;
    char** channels;

    channelMessagesStruct* channelMessages;
    int channelCount;
} pollStruct;

pollStruct pollStruct_initialize(char **channels, int timestamp);
void pollStruct_free(pollStruct *ps);

int handlePollCommand(commandStruct cmd, int sockfd);

#endif //DUDESERVER_MESSAGES_H
