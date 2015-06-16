/*
 * Command: JOIN
 * JOIN <channel> :<#password>
 *
 * Joint <channel>.
 * Indien <#password> meegegeven is, wordt deze gebruikt om te authenticeren.
 */
#include "join.h"

int handleJoinCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 1, cmd.sender);
    }

    userInfo user = getClient(cmd.sender)->user;
    char *channelName = cmd.parameters[0], *optionalChannelKey = cmd.trailing;
    int joinChannelResult;
    int boolUserNeedsToBeOperator = BOOL_FALSE;
    channelInfo channel;
    if(getChannelByName(channelName, &channel) == BOOL_FALSE)
    {
        insertChannel(channelName, NULL, NULL, BOOL_TRUE, 0, 0);
        boolUserNeedsToBeOperator = BOOL_TRUE;
    }
    else
    {
        if(channel.inviteOnly == BOOL_TRUE)
        {
            channelInfo_free(&channel);
            return ERR_INVITEONLYCHAN;
        }

        int result = authenticateChannel(channel, channelName, optionalChannelKey);
        channelInfo_free(&channel);
        if(result != BOOL_TRUE)
        {
            return result;
        }
    }
    joinChannelResult = joinChannelByUsername(channelName, user.username);
    if (boolUserNeedsToBeOperator == BOOL_TRUE)
    {
        updateChannelUserRole(channelName, user.username, USER_ROLE_OPERATOR);
    }

    if(joinChannelResult == RPL_JOIN_CHANNEL)
    {
        char* buffer = MALLOC(INNER_BUFFER_LENGTH);
        sprintf(buffer, "%i %s %s", RPL_JOIN_CHANNEL, channelName, user.username);
        sendToAllClientsInChannel(buffer, channelName);
        FREE(buffer);
    }

    return joinChannelResult;
}

int authenticateChannel(channelInfo channel, char *channelName, char *optionalChannelKey)
{
    if(strcmp(channel.name, channelName) == 0)
    {
        if(channel.password != NULL && strlen(channel.password) > 0)
        {
            if(optionalChannelKey != NULL && strlen(optionalChannelKey) > 0)
            {
                if(strcmp(channel.password, optionalChannelKey) != 0)
                {
                    return ERR_BADCHANNELKEY;
                }
            }
            else
            {
                return ERR_BADCHANNELKEY;
            }
        }
        return BOOL_TRUE;
    }

    return ERR_BADCHANMASK;
}

int joinChannelByUsername(char* channelName, char *username)
{
    if(isUserInChannel(channelName, username) == BOOL_TRUE)
    {
        return RPL_NOREPLY;
    }

    if(userJoinChannel(username, channelName, USER_ROLE_USER) == DB_RETURN_DOESNOTEXIST)
    {
        return ERR_BADCHANMASK;
    }

    char *joinedChanMsg = " has joined the channel!!";
    char *stringToSend = MALLOC(strlen(username) + strlen(joinedChanMsg) + 1);
    sprintf(stringToSend, "%s%s", username, joinedChanMsg);
    sendSystemMessageToChannel(stringToSend, channelName);
    FREE(stringToSend);

    int clientId = getClientIdByUsername(username);
    if(clientId != -1)
    {
        handleChannelNames(channelName, clientId);
    }

    return RPL_JOIN_CHANNEL;
}