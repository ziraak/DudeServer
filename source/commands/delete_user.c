/*
 * Command: DELETE_USER
 * DELETE_USER
 *
 * Verwijdert de huidige gebruiker.
 */
#include "delete_user.h"

//TODO verificatie a.d.h.v. password? iets.. veiliger, lijkt me
int handleDeleteUserCommand(commandStruct cmd)
{
    userInfo user = getClient(cmd.sender)->user;

    if (checkIfUserExists(user.username) == BOOL_TRUE)
    {
        deleteUser(user.username);
        return RPL_SUCCESS;
    }
    ERROR_USERNAME_NOT_KNOWN(user.username, cmd.sender);
}
