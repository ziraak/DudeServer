/*
 * Command: UPDATE_PASSWORD
 * UPDATE_PASSWORD <password>
 *
 * Update het password van de huidige gebruiker naar <password>.
 */
#include "update_password.h"

int handleUpdatePasswordCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 1)
    {
        timeEnd("update_password.c");
        return ERR_NEEDMOREPARAMS;
    }

    if (strlen(cmd.parameters[0]) < MINIMUM_PASSWORD_LENGTH)
    {
        timeEnd("update_password.c");
        return ERR_PASSWORDTOOSHORT;
    }

    if (checkIfUserExists(currentUser.username) == BOOL_TRUE)
    {
        changePassword(currentUser.username, cmd.parameters[0]);
        timeEnd("update_password.c");
        return RPL_SUCCESS;
    }
    timeEnd("update_password.c");
    return ERR_USERNAME_NOT_KNOWN;
}