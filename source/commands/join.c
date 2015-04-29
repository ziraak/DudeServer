#include "join.h"

extern userInfo currentUser;

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

    int result = authenticateChannel(channel, channelName, optionalChannelKey);
    if(result != BOOL_TRUE)
    {
        return result;
    }

    return joinChannel(channelName);
}

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey)
{
    if(optionalChannelKey != NULL && strlen(optionalChannelKey) > 0)
    {
        //TODO: optionalChannelKey support toevoegen
        return ERR_BADCHANNELKEY;
    }

    if(strcmp(channel.name, channelName) == 0)
    {
        return BOOL_TRUE;
    }

    return ERR_BADCHANMASK;
}

int joinChannel(char* channelName)
{
    printf("JOINING %s\n", channelName);
    if(userJoinChannel(currentUser.username, channelName) == BOOL_FALSE)
    {
        return ERR_BADCHANMASK;
    }

    return RPL_TOPIC;
}

int createChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: create channel
    return 0;
}
