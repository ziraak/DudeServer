

#include "poll.h"

char* copy(char* src)
{
    size_t l = strlen(src);
    char* result = malloc(l + 1);
    bzero(result, l + 1);
    strncpy(result, src, l);

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

    *str = malloc(11 + strlen(channelName) + strlen(writer) + strlen(timestamp) + strlen(body));
    sprintf(*str, "UNREAD %s %s %s :%s", channelName, writer, timestamp, body);

    free(writer);
    free(body);
    free(timestamp);

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

    int getMessagesOnTimeResult;
    messageInfo *messageInfos = getMessagesOnTime(channelName, timestamp, &getMessagesOnTimeResult);

    if(messageInfos == NULL)
    {
        channelMessagesStruct result = { .channelName = innerChannelName };
        messageInfo_free(messageInfos);
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
            messages[resultCount] = message;
            resultCount++;
        }
        messageCount++;
    }
    messages[resultCount] = NULL;
    channelMessagesStruct result = {
            .messages = messages,
            .messageCount = resultCount,
            .channelName = innerChannelName
    };
    messageInfo_free(messageInfos);
    return result;
}

int getPollMessages(pollStruct *ps)
{
    if(ps->channelCount == 0)
    {
        return BOOL_FALSE;
    }

    int i;
    channelMessagesStruct *channelMessages = malloc(sizeof(channelMessagesStruct));
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

pollStruct pollStruct_initialize(channelInfo *channels, int channelCount, int timestamp)
{
    char **innerChannels = malloc(sizeof(char));

    int i;
    for(i = 0; i < channelCount; i++)
    {
        innerChannels[i] = malloc(strlen(channels[i].name));
        sprintf(innerChannels[i], "%s", channels[i].name);
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

    int channelCount;
    channelInfo *channels = getUserChannels(currentUser.username, &channelCount);
    pollStruct ps = pollStruct_initialize(channels, 0, channelCount);
    channelInfos_free(channels, channelCount);

    if(getPollMessages(&ps) == BOOL_TRUE)
    {
        sendPollMessages(&ps);
    }

    pollStruct_free(&ps);
    return RPL_SUCCESS;
}
