#ifndef DUDESERVER_MESSAGES_H
#define DUDESERVER_MESSAGES_H

#include "../main.h"
#include "../ssl/communicationStructs.h"
#include "../database/databaseStructs.h"
#include "../database/databaseStructs_free.h"

typedef struct channelMessagesStruct
{
    char** messages;
    int messageCount;
} channelMessagesStruct;

typedef struct pollStruct
{
    int timestamp;

    channelInfo *channels;
    channelMessagesStruct* channelMessages;
    int channelCount;
} pollStruct;

pollStruct pollStruct_initialize(channelInfo *channels, int channelCount, int timestamp);
void pollStruct_free(pollStruct *ps);

int handlePollCommand(commandStruct cmd, int amountOfMessages);

#endif //DUDESERVER_MESSAGES_H
