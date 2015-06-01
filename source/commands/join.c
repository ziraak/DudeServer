#include "join.h"

int handleJoinCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *optionalChannelKey = cmd.trailing;

    channelInfo channel;
    int resultGetChannel = getChannelByName(channelName, &channel);
    if(resultGetChannel == DB_RETURN_DOESNOTEXIST)
    {
        createNewChannel(channelName, NULL, NULL, BOOL_TRUE);
    }
    else if (resultGetChannel == DB_RETURN_SUCCES)
    {
        int result = authenticateChannel(channel, channelName, optionalChannelKey);
        if(result != BOOL_TRUE)
        {
            channelInfo_free(&channel);
            return result;
        }
    }
    else
    {
        channelInfo_free(&channel);
        return resultGetChannel;
    }
    channelInfo_free(&channel);
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
                    return ERR_BADCHANNELKEY;
                }
            }
            else
            {
                return ERR_BADCHANNELKEY;
            }
        }

        return BOOL_TRUE;
    }

    return ERR_BADCHANMASK;
}

int joinChannel(char* channelName)
{
    joinChannelByUsername(channelName, currentUser.username);
}


int joinChannelByUsername(char* channelName, char *username)
{
    if(userHasChannel(channelName) == BOOL_TRUE)
    {
        return RPL_TOPIC;
    }

    if(userJoinChannel(username, channelName, NULL) == DB_RETURN_DOESNOTEXIST)
    {
        return ERR_BADCHANMASK;
    }

    userAddChannel(channelName);

    return RPL_TOPIC;
}

int userHasChannel(char* channelName)
{
    int j;
    for(j = 0; currentUser.channels[j] != NULL; j++)
    {
        if(strcmp(channelName, currentUser.channels[j]) == 0)
        {
            return BOOL_TRUE;
        }
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