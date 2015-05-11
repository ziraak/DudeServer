#include "delete_user.h"

int handleDeleteUserCommand(char *username)
{
    if (checkUser(username) == BOOL_FALSE)
    {
        deleteUser(username);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}
