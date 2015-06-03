#include "invite.h"

int handleInviteCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 2)
    {
        timeEnd("invite.c");
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
        timeEnd("invite.c");
        return resultCheckChannel;
    }
    timeEnd("invite.c");
    return resultCheckChannelHost;
}