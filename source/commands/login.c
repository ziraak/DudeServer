/*
 * Command: LOGIN
 * LOGIN <username> <#nickname> :<password>
 *
 * Logt de gebruiker in als <username> met <password>.
 * Indien <#nickname> meegegeven is, is dit de nieuwe nickname.
 */
#include "login.h"

int handleLoginCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        timeEnd("login.c");
        return ERR_NEEDMOREPARAMS;
    }

    char *username = cmd.parameters[0];
    char *password = cmd.trailing;

    userInfo user;
    int userAuthenticated = authenticateUser(username, password, &user);
    if (userAuthenticated == RPL_LOGIN)
    {
        currentUser = user;
    }

    if(cmd.parameterCount > 1)
    {
        // TODO: doe iets met nickname op cmd.parameters[1]
    }
    timeEnd("login.c");
    return userAuthenticated;
}

int authenticateUser(char *username, char *password, userInfo *result)
{
    timeStart;
    if (getUser(username, result) != DB_RETURN_SUCCES)
    {
        timeEnd("login/authenticateUser");
        return ERR_NOLOGIN;
    }

    if (strcmp(result->username, username) == 0 && strcmp(result->password, password) == 0)
    {
        timeEnd("login/authenticateUser");
        return RPL_LOGIN;
    }

    timeEnd("login/authenticateUser");
    return ERR_NOLOGIN;
}
