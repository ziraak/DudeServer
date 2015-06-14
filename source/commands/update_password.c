/*
 * Command: UPDATE_PASSWORD
 * UPDATE_PASSWORD <password>
 *
 * Update het password van de huidige gebruiker naar <password>.
 */
#include "update_password.h"

int handleUpdatePasswordCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 1, cmd.sender);
    }

    if (strlen(cmd.parameters[0]) < MINIMUM_PASSWORD_LENGTH)
    {
        ERROR_PASSWORD_TOO_SHORT(cmd.sender);
    }

    userInfo user = getClient(cmd.sender)->user;
    if (checkIfUserExists(user.username) == BOOL_TRUE)
    {
        changePassword(user.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }

    ERROR_USERNAME_NOT_KNOWN(user.username, cmd.sender);
}