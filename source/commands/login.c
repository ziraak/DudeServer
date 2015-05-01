//
// Created by osboxes on 21/04/15.
//

#include "login.h"

int handleLoginCommand(char *message)
{
    char *username, *password, *nickname;
    int offset = substringCharacter(message, &username);
    offset = substringCharacter(message += offset, &password);
    substringCharacter(message += offset, &nickname);

    userInfo user;
    int userAuthenticated = authenticateUser(username, password, &user);
    if (userAuthenticated == RPL_LOGIN)
    {
        // TODO: generate token for user
        currentUser = user;
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
