#include "update_password.h"

int handleUpdatePasswordCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    if (strlen(cmd.parameters[0]) < MINIMUM_PASSWORD_LENGTH)
    {
        return ERR_PASSWORDTOOSHORT;
    }

    if (checkIfUserExists(currentUser.username) == BOOL_TRUE)
    {
        changePassword(currentUser.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}