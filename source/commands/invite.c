#include "invite.h"

int hostAllowedToInvite(char *channelName, int client)
{
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        ERROR_NO_SUCH_CHANNEL(channelName, client);
    }

    userInfo user = getClient(client)->user;

    if (userIsOperatorInChannel(channelName, user.username) == BOOL_FALSE)
    {
        ERROR_CHANNEL_PRIVILEGES_NEEDED(channelName, client);
    }

    if(isUserInChannel(channelName, user.username) == BOOL_TRUE)
    {
        return BOOL_TRUE;
    }

    ERROR_NOT_ON_CHANNEL(channelName, user.username, client);
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