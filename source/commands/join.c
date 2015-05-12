#include "join.h"

int handleJoinCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *optionalChannelKey = NULL;

    if(cmd.parameterCount > 1)
    {
        optionalChannelKey = cmd.parameters[1];
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
    if(userHasChannel(channelName) == DB_RETURN_SUCCES)
    {
        return RPL_TOPIC;
    }

    if(userJoinChannel(currentUser.username, channelName) == DB_RETURN_DOESNOTEXIST)
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