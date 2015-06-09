/*
 * Command: PART
 * PART <channel>
 *
 * Vertrek uit <channel>.
 */
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
        userLeaveChannel(currentUser.username, channelName);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannel(channelName);
        }
        else
        {
            char *kickChanMsg = " has left the channel!!";
            char *stringToSend = MALLOC(sizeof(char *) + strlen(currentUser.username) + strlen(kickChanMsg));
            sprintf(stringToSend, "%s%s", currentUser.username, kickChanMsg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);
        }
        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}