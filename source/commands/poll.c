#include "poll.h"

char* copy(char* src)
{
    size_t l = strlen(src) + 1;
    char* result = malloc(l);
    bzero(result, l);
    strncpy(result, src, l - 1);

    return result;
}

int convertChannelMessageToString(messageInfo msg,  char* channelName, char** str)
{
    if(msg.timestamp == NULL || msg.writer == NULL || msg.body == NULL || channelName == NULL)
    {
        return BOOL_FALSE;
    }

    char *writer = copy(msg.writer),
            *body = copy(msg.body),
            *timestamp = copy(msg.timestamp);

    *str = malloc(12 + strlen(channelName) + strlen(writer) + strlen(timestamp) + strlen(body));
    sprintf(*str, "UNREAD %s %s %s :%s", channelName, writer, timestamp, body);

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

int getPollMessages(pollStruct *ps)
{
    if(ps->channelCount == 0)
    {
        return BOOL_FALSE;
    }

    int i;
    channelMessagesStruct *channelMessages = malloc(1);
    for(i = 0; i < ps->channelCount; i++)
    {
        channelMessages[i] = getChannelMessages(ps->channels[i], ps->timestamp);
    }
    ps->channelMessages = channelMessages;

    return BOOL_TRUE;
}

int sendPollMessages(pollStruct *ps)
{
    int i, j;

    for(i = 0; i < ps->channelCount; i++)
    {
        for(j = 0; j < ps->channelMessages[i].messageCount; j++)
        {
            sslSend(ps->channelMessages[i].messages[j]);
        }
    }
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

void pollStruct_free(pollStruct *ps)
{
    if(ps == NULL)
    {
        return;
    }

    if(ps->channels != NULL)
    {
        free(ps->channels);
    }

    if(ps->channelMessages != NULL)
    {
        int i;
        for(i = 0; i < ps->channelCount; i++)
        {
            if(ps->channelMessages[i].channelName != NULL)
            {
                free(ps->channelMessages[i].channelName);
            }

            int j = 0;
            while(ps->channelMessages[i].messages[j] != NULL)
            {
                free(ps->channelMessages[i].messages[j]);
                j++;
            }
        }
    }
}

int handlePollCommand(commandStruct cmd)
{
    if(cmd.parameterCount == 0)
    {
        return ERR_NEEDMOREPARAMS;
    }

    pollStruct ps = pollStruct_initialize(currentUser.channels, atoi(cmd.parameters[0]));

    if(getPollMessages(&ps) == BOOL_TRUE)
    {
        sendPollMessages(&ps);
    }

    pollStruct_free(&ps);
    return RPL_SUCCESS;
}
