#include "join.h"

int handleJoinCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *optionalChannelKey = cmd.trailing; //;

    channelInfo channel;
    int resultGetChannel = getChannel(channelName, &channel);
    if(resultGetChannel == DB_RETURN_DOESNOTEXIST)
    {
        createNewChannel(channelName, NULL, NULL, BOOL_TRUE);
    }
    else if (resultGetChannel == DB_RETURN_SUCCES)
    {
        int result = authenticateChannel(channel, channelName, optionalChannelKey);
        if(result != BOOL_TRUE)
        {
            return result;
        }
    }
    else
    {
        return resultGetChannel;
    }

    return joinChannel(channelName);
}

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey)
{
    if(strcmp(channel.name, channelName) == 0)
    {
        if(channel.password != NULL && strlen(channel.password) > 0)
        {
            if(optionalChannelKey != NULL && strlen(optionalChannelKey) > 0)
            {
                if(strcmp(channel.password, optionalChannelKey) != 0)
                {
                    //TODO: optionalChannelKey support toevoegen
                    return ERR_BADCHANNELKEY;
                }
            }
            else
            {
                return ERR_NEEDMOREPARAMS;
            }
        }

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