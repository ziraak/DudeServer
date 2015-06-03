/*
 * Command: CREATE_USER
 * CREATE_USER <username> :<password>
 *
 * Maak een gebruiker aan met <username> en <password>.
 */
#include "create_user.h"

int handleCreateUserCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        timeEnd("create_user.c");
        return ERR_NEEDMOREPARAMS;
    }

    char *username = cmd.parameters[0],
            *password = cmd.trailing;

    if (checkIfUserExists(username) == BOOL_TRUE)
    {
        timeEnd("create_user.c");
        return ERR_USERNAMEINUSE;
    }
    else
    {
        if (strlen(password) >= MINIMUM_PASSWORD_LENGTH)
        {
            createNewUser(username, password);
            timeEnd("create_user.c");
            return RPL_SUCCESS;
        }
        else
        {
            timeEnd("create_user.c");
            return ERR_PASSWORDTOOSHORT;
        }
    }
    timeEnd("create_user.c");
}