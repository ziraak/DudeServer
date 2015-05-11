#include "update_password.h"

int handleUpdatePasswordCommand(char *username, char *password)
{
    if (checkUser(username) == BOOL_TRUE)
    {
        changePassword(username, password);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}