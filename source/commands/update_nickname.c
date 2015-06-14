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

    userInfo user = getClient(cmd.sender)->user;

    if (checkIfUserExists(user.username) == BOOL_TRUE)
    {
        changeNickname(user.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}