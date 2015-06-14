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
        return ERR_PASSWORDTOOSHORT;
    }

    if (checkIfUserExists(getClient(cmd.sender)->user.username) == BOOL_TRUE)
    {
        changePassword(getClient(cmd.sender)->user.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}