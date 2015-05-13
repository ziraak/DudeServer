#include "messages.h"

void convertChannelMessageToString(messageInfo msg,  char* channelName, char** str)
{
    *str = malloc(12 + strlen(channelName) + strlen(msg.writer) + strlen(msg.timestamp) + strlen(msg.body));
    sprintf(*str, "UNREAD %s %s %s :%s\0", channelName, msg.writer, msg.timestamp, msg.body);
}

channelMessagesStruct getChannelMessages(char* channelName, int timestamp)
{
    channelMessagesStruct cms;

    cms.channelName = channelName;

    messageInfo* messages = getMessagesOnTime(channelName, timestamp);

    char** msgs = malloc(sizeof(char));
    int messageCount = 0;
    while(messages[messageCount].writer != NULL && messages[messageCount].body != NULL)
    {
        char* msg;
        convertChannelMessageToString(messages[messageCount], channelName, &msg);

        msgs[messageCount] = malloc(sizeof(char) * strlen(msg));
        strcpy(msgs[messageCount], msg);
        free(msg);

        messageInfo_free(&messages[messageCount]);
        messageCount++;
    }

    cms.messages = msgs;
    cms.messageCount = messageCount;

    // TODO: free alle messageInfo's

    return cms;
}

int getAllUnreadMessages(getMessagesStruct *gms)
{
    if(gms->channelCount == 0)
    {
        return BOOL_FALSE;
    }

    int i;
    channelMessagesStruct *channelMessages = malloc(1);
    for(i = 0; i < gms->channelCount; i++)
    {
        channelMessages[i] = getChannelMessages(gms->channels[i], gms->timestamp);
    }
    gms->channelMessages = channelMessages;

    return BOOL_TRUE;
}

int processMessages(getMessagesStruct *gms, int sockfd)
{
    int i, j;

    for(i = 0; i < gms->channelCount; i++)
    {
        for(j = 0; j < gms->channelMessages[i].messageCount; j++)
        {
            sendMessageToClient(sockfd, gms->channelMessages[i].messages[j], strlen(gms->channelMessages[i].messages[j]));
        }
    }

    getMessagesStruct_free(gms);

    return BOOL_TRUE;
}

getMessagesStruct getMessagesStruct_initialize(char** channels, int timestamp)
{
    getMessagesStruct gms;
    gms.timestamp = timestamp;
    gms.channels = malloc(sizeof(char));

    int i = 0;
    while(channels[i] != NULL)
    {
        gms.channels[i] = malloc(strlen(channels[i]) + 1);
        sprintf(gms.channels[i], "%s", channels[i]);
        i++;
    }

    gms.channelCount = i;

    return gms;
}

void getMessagesStruct_free(getMessagesStruct *gms)
{
    if(gms == NULL)
    {
        return;
    }

    if(gms->channels != NULL)
    {
        free(gms->channels);
    }

    if(gms->channelMessages != NULL)
    {
        int i;
        for(i = 0; i < gms->channelCount; i++)
        {
            free(gms->channelMessages[i].channelName);
            free(gms->channelMessages[i].messages);
        }
    }
}
