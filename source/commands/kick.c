#include "kick.h"

int handleKickCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    char *usernameToKick = cmd.parameters[1];

    if (checkChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        return ERR_CHANOPPRIVSNEEDED;
    }

    if(isUserInChannel(channelName, currentUser.username) == BOOL_TRUE)
    {
        userLeaveChannel(usernameToKick, channelName);
        return RPL_SUCCESS;
    }
    return ERR_NOTONCHANNEL;
}