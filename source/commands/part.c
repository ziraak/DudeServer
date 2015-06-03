/*
 * Command: PART
 * PART <channel>
 *
 * Vertrek uit <channel>.
 */
#include "part.h"

int handlePartCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 1)
    {
        timeEnd("part.c");
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0];

    if (checkChannel(channelName) == BOOL_TRUE)
    {
        userLeaveChannel(currentUser.username, channelName);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannel(channelName);
        }
        timeEnd("part.c");
        return RPL_SUCCESS;
    }
    timeEnd("part.c");
    return ERR_NOSUCHCHANNEL;
}