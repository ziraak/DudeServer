//
// Created by osboxes on 21/04/15.
//

#include "login.h"
#include "../main.h"
#include "../utils/utils.h"

int handleLoginCommand(char *message) {
    char *username, *password, *nickname;
    int offset = substringCharacter(message, &username);
    offset = substringCharacter(message += offset, &password);
    substringCharacter(message += offset, &nickname);

    int userAuthenticated = authenticateUser(username, password);
    if (userAuthenticated) {
        // TODO: Set id somewhere to know that this user is authenticated and may communicate with the server
        return RPL_LOGIN;
    }

    return ERR_NOLOGIN;
}

int authenticateUser(char *username, char *password) {
    // TODO: Check for user in DB
    return 0;
}
