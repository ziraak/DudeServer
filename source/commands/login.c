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

    int userAuthenticated = authenticateUser(username, password);
    if (userAuthenticated == RPL_LOGIN)
    {
        // TODO: generate token for user
    }

    return userAuthenticated;
}

int authenticateUser(char *username, char *password)
{
    userInfo user;
    if(getUser(username, &user) < 0)
    {
        return ERR_NOLOGIN;
    }

    if(strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0)
    {
        return RPL_LOGIN;
    }

    return ERR_NOLOGIN;
}
