#include "kick.h"

int handleKickCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    char *usernameToKick = cmd.parameters[1];

    if (checkChannel(channelName) == BOOL_FALSE)
    {
        ERROR_NO_SUCH_CHANNEL(channelName, cmd.client);
    }

    userInfo user = getClient(cmd.client)->user;

    if (userIsOperatorInChannel(channelName, user.username) == BOOL_FALSE)
    {
        ERROR_CHANNEL_PRIVILEGES_NEEDED(channelName, cmd.client);
    }

    if(isUserInChannel(channelName, user.username) == BOOL_TRUE)
    {
        userLeaveChannel(usernameToKick, channelName);

        if (checkIfChannelEmpty(channelName))
        {
            deleteChannel(channelName);
        }
        else
        {
            REPLY_PART(channelName, usernameToKick);

            char *kickChanMsg = " was kicked out of the channel!!";
            char *stringToSend = MALLOC(strlen(usernameToKick) + strlen(kickChanMsg) + 1);
            sprintf(stringToSend, "%s%s", usernameToKick, kickChanMsg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);
        }

        return RPL_SUCCESS;
    }

    ERROR_NOT_ON_CHANNEL(channelName, user.username, cmd.client);
}