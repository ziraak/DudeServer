#include "kick.h"

int handleKickCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    char *usernameToKick = cmd.parameters[1];

    if (checkChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, getClient(cmd.sender)->user.username) == BOOL_FALSE)
    {
        return ERR_CHANOPPRIVSNEEDED;
    }

    if(isUserInChannel(channelName, getClient(cmd.sender)->user.username) == BOOL_TRUE)
    {
        userLeaveChannel(usernameToKick, channelName);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannel(channelName);
        }
        else
        {
            char *kickChanMsg = " was kicked out of the channel!!";
            char *stringToSend = MALLOC(strlen(usernameToKick) + strlen(kickChanMsg) + 1);
            sprintf(stringToSend, "%s%s", usernameToKick, kickChanMsg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);
        }

        return RPL_SUCCESS;
    }
    return ERR_NOTONCHANNEL;
}