/*
 * Command: CREATE_USER
 * CREATE_USER <username> :<password>
 *
 * Maak een gebruiker aan met <username> en <password>.
 */
#include "create_user.h"

int handleCreateUserCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.sender);
    }

    char *username = cmd.parameters[0],
            *password = cmd.trailing;

    if (checkIfUserExists(username) == BOOL_TRUE)
    {
        return ERR_USERNAMEINUSE;
    }
    else
    {
        if (strlen(password) >= MINIMUM_PASSWORD_LENGTH)
        {
            createNewUser(username, password);
            return RPL_SUCCESS;
        }
        else
        {
            ERROR_PASSWORD_TOO_SHORT(cmd.sender);
        }
    }
}