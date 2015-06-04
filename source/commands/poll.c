#include "poll.h"

int convertChannelMessageToString(messageInfo msg,  char* channelName, char** str)
{
    timeStart;
    if(msg.timestamp == NULL || msg.writer == NULL || msg.body == NULL || channelName == NULL)
    {
        timeEnd("convertChannelMsg2Str");
        return BOOL_FALSE;
    }

    *str = MALLOC(12 + strlen(channelName) + strlen(msg.writer) + strlen(msg.timestamp) + strlen(msg.body));
    sprintf(*str, "UNREAD %s %s %s :%s", channelName, msg.writer, msg.timestamp, msg.body);

    timeEnd("convertChannelMsg2Str");
    return BOOL_TRUE;
}

channelMessagesStruct getChannelMessages(channelInfo channel, int timestamp, int amountOfMessages)
{
    timeStart;
    if(channel.name == NULL)
    {
        channelMessagesStruct result;
        timeEnd("poll.c/getChannelMsgs");
        return result;
    }

    int getMessagesOnTimeResult;
    messageInfo *messageInfos = getMessagesOnTime(channel.name, timestamp, &getMessagesOnTimeResult, amountOfMessages);

    char **messages = MALLOC(sizeof(char*) * getMessagesOnTimeResult);
    int messageCount = 0,
        resultCount = 0;
    while(messageCount < getMessagesOnTimeResult)
    {
        char *message;

        if(convertChannelMessageToString(messageInfos[messageCount], channel.name, &message) == BOOL_TRUE)
        {
            messages[resultCount] = message;
            resultCount++;
        }
        messageCount++;
    }

    channelMessagesStruct result;
    result.messages = messages;
    result.messageCount = resultCount;
    messageInfos_free(messageInfos, getMessagesOnTimeResult);
    timeEnd("poll.c/getChannelMsgs");
    return result;
}

int getPollMessages(pollStruct *ps, int amountOfMessages)
{
    timeStart;
    if(ps->channelCount == 0)
    {
        timeEnd("getPollMsgs");
        return BOOL_FALSE;
    }

    int i;
    ps->channelMessages = MALLOC(sizeof(channelMessagesStruct) * ps->channelCount);
    for(i = 0; i < ps->channelCount; i++)
    {
        ps->channelMessages[i] = getChannelMessages(ps->channels[i], ps->timestamp, amountOfMessages);
    }
    timeEnd("getPollMsgs");
    return BOOL_TRUE;
}

int sendPollMessages(pollStruct *ps)
{
    timeStart;
    int i, j;

    for(i = 0; i < ps->channelCount; i++)
    {
        for(j = 0; j < ps->channelMessages[i].messageCount; j++)
        {
            sslSend(ps->channelMessages[i].messages[j]);
        }
    }
    timeEnd("sendPollMsgs");
    return BOOL_TRUE;//TODO: altijd true?
}

pollStruct pollStruct_initialize(channelInfo *channels, int channelCount, int timestamp)
{
    pollStruct ps = {
        .channels = channels,
        .channelCount = channelCount,
        .timestamp = timestamp
    };

    return ps;
}

void pollStruct_free(pollStruct *ps)
{
    if(ps != NULL)
    {
        int i;
        for(i = 0; i < ps->channelCount; i++)
        {
            int j;
            for(j = 0; j < ps->channelMessages[i].messageCount; j++)
            {
                FREE(ps->channelMessages[i].messages[j]);
            }

            FREE(ps->channelMessages[i].messages);
        }

        FREE(ps->channelMessages);
        channelInfos_free(ps->channels, ps->channelCount);
    }
}

int handlePollCommand(commandStruct cmd, int amountOfMessages)
{
    timeStart;
    if(cmd.parameterCount == 0)
    {

//        return ERR_NEEDMOREPARAMS;

        timeEnd("handlePollCommand");

    }

    int channelCount;
    channelInfo *channels = getUserChannels(currentUser.username, &channelCount);
    pollStruct ps = pollStruct_initialize(channels, channelCount, lastTimestamp);

    if(getPollMessages(&ps, amountOfMessages) == BOOL_TRUE)
    {
        sendPollMessages(&ps);
    }

    pollStruct_free(&ps);
    lastTimestamp = (int)time(NULL);
    timeEnd("handlePollCommand");
    return RPL_SUCCESS;
}
