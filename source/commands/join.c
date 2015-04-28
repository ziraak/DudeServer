//
// Created by osboxes on 21/04/15.
//

#include "join.h"

int handleJoinCommand(char *message)
{
    char *channelName, *optionalChannelKey = NULL;
    int offset = substringCharacter(message, &channelName);
    if (!(*(message + offset) == '\n' || *(message + offset) == '\0'))
    {
        substringCharacter(message += offset, &optionalChannelKey);
    }

    channelInfo channel;
    if(getChannel(channelName, &channel) < 0)
    {
        // TODO: create channel and
        createChannel(channelName, optionalChannelKey);
    }

    int result = authenticateChannel(channel, channelName, optionalChannelKey)
    if(result != BOOL_TRUE)
    {
        return result;
    }
    else
    {
        printf("JOINED!\n");
    }

    joinChannel(channel);

    return RPL_TOPIC;
}

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey)
{
    if(optionalChannelKey != NULL)
    {
        //TODO: optionalChannelKey toevoegen
    }

    if(strcmp(channel.name, channelName) == 0)
    {
        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

int joinChannel(channelInfo channel)
{
    // TODO: Add current user to the channel
    return 0;
}

int createChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: create channel
    return 0;
}
