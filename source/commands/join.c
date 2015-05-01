#include "join.h"

int handleJoinCommand(char *message)
{
    char *channelName = NULL, *optionalChannelKey = NULL;
    int offset = substringCharacter(message, &channelName);
    if (!(*(message + offset) == '\n' || *(message + offset) == '\0'))
    {
        substringCharacter(message += offset, &optionalChannelKey);
    }

    channelInfo channel;
    if(getChannel(channelName, &channel) == DB_RETURN_DOESNOTEXIST)
    {
        createNewChannel(channelName, currentUser.username);
    }
    else
    {
        int result = authenticateChannel(channel, channelName, optionalChannelKey);
        if(result != BOOL_TRUE)
        {
            return result;
        }
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
    if(userHasChannel(channelName) == BOOL_TRUE)
    {
        return RPL_TOPIC;
    }

    if(userJoinChannel(currentUser.username, channelName) == BOOL_FALSE)
    {
        return ERR_BADCHANMASK;
    }

    userAddChannel(channelName);

    return RPL_TOPIC;
}

int userHasChannel(char* channelName)
{
    char** channels = currentUser.channels;

    while(*channels != NULL)
    {
        if(strcmp(channelName, *channels) == 0)
        {
            return BOOL_TRUE;
        }

        channels++;
    }

    return BOOL_FALSE;
}

void userAddChannel(char* channelName)
{
    if(userHasChannel(channelName) == BOOL_FALSE)
    {
        char** channels = currentUser.channels;

        while(*channels != NULL) { channels++; }

        *channels = channelName;
    }
}