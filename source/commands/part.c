/*
 * Command: PART
 * PART <channel>
 *
 * Vertrek uit <channel>.
 */
#include "part.h"

void makeTheLastPersonOperatorIfThereIsOnlyPersonLeftInChannel(char *channelName)
{
    int userCount;
    userInfo *users = getChannelUsers(channelName, &userCount);
    char *str = MALLOC(sizeof(char *) + 9 + strlen(channelName) + strlen(users->username));
    sprintf(str, "MODE %s +o %s", channelName, users->username);
    if (userCount < 2)
    {
        if (userIsOperatorInChannel(channelName, users->username) == BOOL_FALSE)
        {
            updateChannelUserRole(channelName, users->username, USER_ROLE_OPERATOR);
            char *msg = ", because you are the last person in the channel. You are now the operator in this channel!!";
            char *stringToSend = MALLOC(sizeof(char *) + strlen(users->username) + strlen(msg));
            sprintf(stringToSend, "%s%s", users->username, msg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);
        }
    }
    FREE(str);
    userInfos_free(users, userCount);
}

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

        makeTheLastPersonOperatorIfThereIsOnlyPersonLeftInChannel(channelName);

        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}