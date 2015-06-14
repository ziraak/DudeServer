/*
 * Command: UPDATE_CHANNEL_PASSWORD
 * UPDATE_CHANNEL_PASSWORD <channel_name> <password>
 *
 * Update het password van de <channel_name> naar <password>.
 */

#include "update_channel_password.h"

int handleUpdateChannelPasswordCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channelName = cmd.parameters[0],
            *password = cmd.parameters[1];
    userInfo user = getClient(cmd.sender)->user;

    if (strlen(cmd.parameters[1]) < MINIMUM_PASSWORD_LENGTH)
    {
        return ERR_PASSWORDTOOSHORT;
    }

    if (checkIfUserExists(user.username) == BOOL_FALSE)
    {
        return ERR_USERNAME_NOT_KNOWN;
    }

    if (isUserInChannel(channelName, user.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, user.username))
    {
        updateChannelPassword(channelName, password);
    }

    return RPL_SUCCESS;
}