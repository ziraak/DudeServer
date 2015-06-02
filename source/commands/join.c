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
        insertChannel(channelName, NULL, NULL, BOOL_TRUE);
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
    if(isUserInChannel(channelName, username) == BOOL_TRUE)
    {
        return RPL_TOPIC;
    }

    if(userJoinChannel(username, channelName, USER_ROLE_USER) == DB_RETURN_DOESNOTEXIST)
    {
        return ERR_BADCHANMASK;
    }

    return RPL_TOPIC;
}