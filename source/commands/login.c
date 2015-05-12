//
// Created by osboxes on 21/04/15.
//

#include "login.h"

int handleLoginCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *username = cmd.parameters[0],
            *password = cmd.parameters[1];

    userInfo user;
    int userAuthenticated = authenticateUser(username, password, &user);
    if (userAuthenticated == RPL_LOGIN)
    {
        currentUser = user;
    }

    if(cmd.parameterCount > 2)
    {
        // TODO: doe iets met nickname op cmd.parameters[2]
    }

    return userAuthenticated;
}

int authenticateUser(char *username, char *password, userInfo *result)
{
    if (getUser(username, result) < 0)
    {
        return ERR_NOLOGIN;
    }

    if (strcmp(result->username, username) == 0 && strcmp(result->password, password) == 0)
    {
        return RPL_LOGIN;
    }

    return ERR_NOLOGIN;
}
