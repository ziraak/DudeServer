#include "part.h"

int handlePartCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0];

    if (checkChannel(channelName) == BOOL_TRUE)
    {
        deleteUserFromChannel(channelName, currentUser.username);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannelInDB(channelName);
        }
        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}