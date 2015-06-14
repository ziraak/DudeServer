/*
 * Command: UPDATE_CHANNEL_PASSWORD
 * UPDATE_CHANNEL_PASSWORD <channel_name> <password>
 *
 * Update het password van de <channel_name> naar <password>.
 */

#include "update_channel_password.h"

int handleUpdateChannelPasswordCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.sender);
    }

    char *channelName = cmd.parameters[0],
            *password = cmd.trailing;
    userInfo user = getClient(cmd.sender)->user;

    if (strlen(cmd.parameters[1]) < MINIMUM_PASSWORD_LENGTH)
    {
        ERROR_PASSWORD_TOO_SHORT(cmd.sender);
    }

    if (checkIfUserExists(user.username) == BOOL_FALSE)
    {
        ERROR_USERNAME_NOT_KNOWN(user.username, cmd.sender);
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