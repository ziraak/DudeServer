#include "delete_user.h"

int handleDeleteUserCommand()
{
    if (checkIfUserExists(currentUser.username) == BOOL_TRUE)
    {
        deleteUser(currentUser.username);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}
