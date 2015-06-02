/*
 * Command: UPDATE_NICKNAME
 * UPDATE_NICKNAME <nickname>
 *
 * Update de nickname van de huidige gebruiker naar <nickname>.
 */
#include "update_nickname.h"

int handleUpdateNicknameCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    if (checkIfUserExists(currentUser.username) == BOOL_TRUE)
    {
        changeNickname(currentUser.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}