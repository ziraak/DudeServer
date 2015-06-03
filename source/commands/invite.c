#include "invite.h"

int hostAllowedToInvite(char *channelName)
{
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        return ERR_CHANOPPRIVSNEEDED;
    }

    if(isUserInChannel(channelName, currentUser.username) == BOOL_TRUE)
    {
        return BOOL_TRUE;
    }
    return ERR_NOTONCHANNEL;
}

int handleInviteCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *usernameUserToInvite = cmd.parameters[1];

    int resultCheckChannelHost = hostAllowedToInvite(channelName);

    if (resultCheckChannelHost == BOOL_TRUE)
    {
        int resultCheckChannel = checkChannel(channelName);
        if (resultCheckChannel == BOOL_TRUE)
        {
            joinChannelByUsername(channelName, usernameUserToInvite);
        }
        return RPL_INVITING;
    }
    return resultCheckChannelHost;
}