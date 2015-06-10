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
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
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

    return userAuthenticated;
}

int authenticateUser(char *username, char *password, userInfo *result)
{
    if (getUser(username, result) != DB_RETURN_SUCCES)
    {
        return ERR_NOLOGIN;
    }

    password = passwordEncrypt(password);
    printf("password\n%s\n%s\n",password,result->password);
    if (strcmp(result->username, username) == 0 && strcmp(result->password, password) == 0)
    {
        return RPL_LOGIN;
    }

    return ERR_NOLOGIN;
}
