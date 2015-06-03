/*
 * Command: DELETE_USER
 * DELETE_USER
 *
 * Verwijdert de huidige gebruiker.
 */
#include "delete_user.h"

//TODO verificatie a.d.h.v. password? iets.. veiliger, lijkt me
int handleDeleteUserCommand()
{
    timeStart;
    if (checkIfUserExists(currentUser.username) == BOOL_TRUE)
    {
        deleteUser(currentUser.username);
        freeCurrentUser();
        timeEnd("delete_user");
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
    timeEnd("delete_user");
}
