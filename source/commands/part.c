#include "part.h"

int handlePartCommand(char *channelName)
{
    if (checkChannel(channelName) == EXIT_SUCCESS)
    {
        deleteUserFromChannel(channelName, currentUser.username);
        if (checkIfChannelEmpty(channelName))
        {
            deleteChannel(channelName);
            return RPL_SUCCESS;
        }
    }
    return ERR_NOSUCHCHANNEL;
}