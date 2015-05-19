#include "poll.h"

int convertChannelMessageToString(messageInfo msg,  char* channelName, char** str)
{
    if(msg.timestamp == NULL || msg.writer == NULL || msg.body == NULL || channelName == NULL)
    {
        return BOOL_FALSE;
    }

    *str = malloc(12 + strlen(channelName) + strlen(msg.writer) + strlen(msg.timestamp) + strlen(msg.body));
    sprintf(*str, "UNREAD %s %s %s :%s", channelName, msg.writer, msg.timestamp, msg.body);

    return BOOL_TRUE;
}

channelMessagesStruct getChannelMessages(char* channelName, int timestamp)
{
    if(channelName == NULL)
    {
        channelMessagesStruct result;
        return result;
    }

    char *innerChannelName = malloc(strlen(channelName));
    strncpy(innerChannelName, channelName, strlen(channelName));

    messageInfo *messageInfos = getMessagesOnTime(channelName, timestamp);

    if(messageInfos == NULL)
    {
        channelMessagesStruct result = { .channelName = innerChannelName };

        return result;
    }

    char **messages = malloc(sizeof(char));
    int messageCount = 0,
        resultCount = 0;
    while(messageInfos[messageCount].writer != NULL && messageInfos[messageCount].body != NULL)
    {
        char *message;

        if(convertChannelMessageToString(messageInfos[messageCount], channelName, &message) == BOOL_TRUE)
        {
            messages[resultCount] = malloc(sizeof(char) * strlen(message));
            strcpy(messages[resultCount], message);
            free(message);
            resultCount++;
        }

        messageInfo_free(&messageInfos[messageCount]);
        messageCount++;
    }

    messages[resultCount] = NULL;

    channelMessagesStruct result = {
            .messages = messages,
            .messageCount = resultCount,
            .channelName = innerChannelName
    };

    return result;
}

int getPollMessages(pollStruct *gms)
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

int sendPollMessages(pollStruct *gms, int sockfd)
{
    int i, j;

    for(i = 0; i < gms->channelCount; i++)
    {
        for(j = 0; j < gms->channelMessages[i].messageCount; j++)
        {
            sendMessageToClient(sockfd, gms->channelMessages[i].messages[j]);
        }
    }

    pollStruct_free(gms);

    return BOOL_TRUE;
}

pollStruct pollStruct_initialize(char **channels, int timestamp)
{
    char **innerChannels = malloc(sizeof(char));

    int i = 0;
    while(channels[i] != NULL)
    {
        innerChannels[i] = malloc(strlen(channels[i]) + 1);
        sprintf(innerChannels[i], "%s", channels[i]);
        i++;
    }

    innerChannels[i] = NULL;

    pollStruct ps = {
        .channels = innerChannels,
        .channelCount = i,
        .timestamp = timestamp
    };

    return ps;
}

void pollStruct_free(pollStruct *gms)
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
            if(gms->channelMessages[i].channelName != NULL)
            {
                free(gms->channelMessages[i].channelName);
            }

            int j = 0;
            while(gms->channelMessages[i].messages[j] != NULL)
            {
                free(gms->channelMessages[i].messages[j]);
                j++;
            }
        }
    }
}

int handlePollCommand(commandStruct cmd, int sockfd)
{
    if(cmd.parameterCount == 0)
    {
        return ERR_NEEDMOREPARAMS;
    }

    //TODO: atoi afhandeling
    pollStruct gms = pollStruct_initialize(currentUser.channels, atoi(cmd.parameters[0]));

    if(getPollMessages(&gms) == BOOL_TRUE)
    {
        sendPollMessages(&gms, sockfd);
    }

    return RPL_SUCCESS;
}
