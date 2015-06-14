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
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 1, cmd.sender);
    }

    userInfo user = getClient(cmd.sender)->user;
    char *channelName = cmd.parameters[0];

    if (checkChannel(channelName) == BOOL_TRUE)
    {
        userLeaveChannel(user.username, channelName);

        if (checkIfChannelEmpty(channelName) == BOOL_TRUE)
        {
            deleteChannel(channelName);
        }
        else
        {
            char *msg = " has left the channel!!";
            char *stringToSend = MALLOC(strlen(user.username) + strlen(msg) + 1);
            sprintf(stringToSend, "%s%s", user.username, msg);
            sendSystemMessageToChannel(stringToSend, channelName);
            FREE(stringToSend);

            makeTheLastPersonOperatorIfThereIsOnlyOnePersonLeftInChannel(channelName);

            char *buffer = MALLOC(INNER_BUFFER_LENGTH);
            sprintf(buffer, "%i %s %s", RPL_PART_CHANNEL, channelName, user.username);
            sendToAllClients(buffer);
            FREE(buffer);
        }

        return RPL_SUCCESS;
    }
    ERROR_NO_SUCH_CHANNEL(channelName, cmd.sender);
}