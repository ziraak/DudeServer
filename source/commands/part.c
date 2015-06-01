#include "part.h"

void removeChannelFromCurrentUserChannels(const char *channelName);

int handlePartCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0];

    if (checkChannel(channelName) == BOOL_TRUE)
    {
        userLeaveChannel(currentUser.username, channelName);
        removeChannelFromCurrentUserChannels(channelName);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannelInDB(channelName);
        }
        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}

void removeChannelFromCurrentUserChannels(const char *channelName)
{
    int i;
    for(i = 0; currentUser.channels[i] != NULL; i++)
    {
        if (strcmp(channelName, currentUser.channels[i]) == 0)
        {
            strcpy(currentUser.channels[i], "");
        }
    }
}