#ifndef DUDESERVER_MESSAGES_H
#define DUDESERVER_MESSAGES_H

#include "../main.h"
#include "../database/databaseStructs.h"

typedef struct channelMessagesStruct
{
    char* channelName;

    char** messages;
    int messageCount;
} channelMessagesStruct;

typedef struct getMessagesStruct
{
    int timestamp;
    char** channels;

    channelMessagesStruct* channelMessages;
    int channelCount;
} getMessagesStruct;

int getAllUnreadMessages(getMessagesStruct *gms);

int processMessages(getMessagesStruct *gms, int sockfd);

getMessagesStruct getMessagesStruct_initialize(char** channels);

void getMessagesStruct_free(getMessagesStruct *gms);

#endif //DUDESERVER_MESSAGES_H
