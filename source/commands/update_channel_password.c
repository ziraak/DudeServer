/*
 * Command: UPDATE_CHANNEL_PASSWORD
 * UPDATE_CHANNEL_PASSWORD <channel_name> <password>
 *
 * Update het password van de <channel_name> naar <password>.
 */

#include "update_channel_password.h"

int handleUpdateChannelPasswordCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    char *password = cmd.parameters[1];

    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    if (strlen(cmd.parameters[1]) < MINIMUM_PASSWORD_LENGTH)
    {
        return ERR_PASSWORDTOOSHORT;
    }

    if (checkIfUserExists(currentUser.username) == BOOL_FALSE)
    {
        return ERR_USERNAME_NOT_KNOWN;
    }

    if (isUserInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    if (userIsOperatorInChannel(channelName, currentUser.username))
    {
        updateChannelPassword(channelName, password);
    }

    return RPL_SUCCESS;
}