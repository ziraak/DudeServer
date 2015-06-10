/*
 * Command: PART
 * PART <channel>
 *
 * Vertrek uit <channel>.
 */
#include "part.h"

void makeTheLastPersonOperatorIfThereIsOnlyOnePersonLeftInChannel(char *channelName)
{
    int userCount;
    userInfo *users = getChannelUsers(channelName, &userCount);
    if (userCount == 1)
    {
        if (userIsOperatorInChannel(channelName, users->username) == BOOL_FALSE)
        {
            updateChannelUserRole(channelName, users->username, USER_ROLE_OPERATOR);
            char *msg = ", because you are the last person in the channel. You are now the operator in this channel!!";
            char *stringToSend = MALLOC(strlen(users->username) + strlen(msg) + 1);
            sprintf(stringToSend, "%s%s", users->username, msg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);
        }
    }
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

        if (checkIfChannelEmpty(channelName) == BOOL_TRUE)
        {
            deleteChannel(channelName);
        }
        else
        {
            char *msg = " has left the channel!!";
            char *stringToSend = MALLOC(strlen(currentUser.username) + strlen(msg) + 1);
            sprintf(stringToSend, "%s%s", currentUser.username, msg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);

            makeTheLastPersonOperatorIfThereIsOnlyOnePersonLeftInChannel(channelName);
        }

        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}