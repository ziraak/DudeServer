#include "invite.h"

int handleInviteCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0], *usernameUserToInvite = cmd.parameters[1];

    int resultCheckChannelHost = checkChannel(currentUser.username);

    if (resultCheckChannelHost == BOOL_TRUE)
    {
        int resultCheckChannel = checkChannel(channelName);
        if (resultCheckChannel == BOOL_TRUE)
        {
            joinChannelByUsername(channelName, usernameUserToInvite);
        }
        return resultCheckChannel;
    }
    return resultCheckChannelHost;
}