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

    char** msgs = malloc(1);
    int messageCount = 0;
    while(messages->writer != NULL && messages->body != NULL)
    {
        char* msg;
        convertChannelMessageToString(*messages, channelName, &msg);

        msgs[messageCount] = malloc(sizeof(char) * strlen(msg));
        strcpy(msgs[messageCount], msg);
        free(msg);

        messages++;
        messageCount++;
    }

    cms.messages = msgs;
    cms.messageCount = messageCount;

    return cms;
}

int getAllUnreadMessages(getMessagesStruct *gms)
{
    if(gms->channelCount == 0)
    {
        return BOOL_TRUE;
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

    return BOOL_TRUE;
}

getMessagesStruct getMessagesStruct_initialize(char** channels)
{
    getMessagesStruct gms;

    gms.channels = channels;
    gms.channelCount = 0;

    while(*channels != NULL)
    {
        gms.channelCount++; channels++;
    }

    return gms;
}

void getMessagesStruct_free(getMessagesStruct *gms)
{

}
