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

        char *kickChanMsg = " was kicked out of the channel!!";
        char *stringToSend = MALLOC(sizeof(char *) + strlen(usernameToKick) + strlen(kickChanMsg));
        sprintf(stringToSend, "%s%s", usernameToKick, kickChanMsg);
        sendSystemMessageToChannel(stringToSend, channelName);
        FREE(stringToSend);

        return RPL_SUCCESS;
    }
    return ERR_NOTONCHANNEL;
}