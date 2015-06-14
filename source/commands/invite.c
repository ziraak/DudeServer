#include "invite.h"

int hostAllowedToInvite(char *channelName, int client)
{
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, getClient(client)->user.username) == BOOL_FALSE)
    {
        return ERR_CHANOPPRIVSNEEDED;
    }

    if(isUserInChannel(channelName, getClient(client)->user.username) == BOOL_TRUE)
    {
        return BOOL_TRUE;
    }
    return ERR_NOTONCHANNEL;
}

int handleInviteCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.sender);
    }

    char *channelName = cmd.parameters[0], *usernameUserToInvite = cmd.parameters[1];

    if (checkIfUserExists(usernameUserToInvite) == BOOL_FALSE)
    {
        ERROR_USERNAME_NOT_KNOWN(usernameUserToInvite, cmd.sender);
    }

    int resultCheckChannelHost = hostAllowedToInvite(channelName, cmd.sender);

    if (resultCheckChannelHost == BOOL_TRUE)
    {
        joinChannelByUsername(channelName, usernameUserToInvite);
        return RPL_INVITING;
    }
    return resultCheckChannelHost;
}