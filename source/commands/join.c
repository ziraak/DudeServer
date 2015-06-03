/*
 * Command: JOIN
 * JOIN <channel> :<#password>
 *
 * Joint <channel>.
 * Indien <#password> meegegeven is, wordt deze gebruikt om te authenticeren.
 */
#include "join.h"

int handleJoinCommand(commandStruct cmd)
{
    timeStart
    if(cmd.parameterCount < 1)
    {
        timeEnd("join.c command");
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *optionalChannelKey = cmd.trailing;

    channelInfo channel;
    if(getChannelByName(channelName, &channel) == BOOL_FALSE)
    {
        insertChannel(channelName, NULL, NULL, BOOL_TRUE);
        channelInfo_free(&channel);
    }
    else
    {
        int result = authenticateChannel(channel, channelName, optionalChannelKey);
        channelInfo_free(&channel);
        if(result != BOOL_TRUE)
        {
            timeEnd("join.c command");
            return result;
        }
    }
    timeEnd("join.c command");
    return joinChannel(channelName);
}

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey)
{
    timeStart;
    if(strcmp(channel.name, channelName) == 0)
    {
        if(channel.password != NULL && strlen(channel.password) > 0)
        {
            if(optionalChannelKey != NULL && strlen(optionalChannelKey) > 0)
            {
                if(strcmp(channel.password, optionalChannelKey) != 0)
                {
                    timeEnd("join/authenticateChannel");
                    return ERR_BADCHANNELKEY;
                }
            }
            else
            {
                timeEnd("join/authenticateChannel");
                return ERR_BADCHANNELKEY;
            }
        }
        timeEnd("join/authenticateChannel");
        return BOOL_TRUE;
    }

    timeEnd("join/authenticateChannel");
    return ERR_BADCHANMASK;
}

int joinChannel(char* channelName)
{
    timeStart;
    joinChannelByUsername(channelName, currentUser.username);
    timeEnd("join.c/joinChannelByUsername");
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