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
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 1, cmd.sender);
    }

    userInfo user = getClient(cmd.sender)->user;

    if (checkIfUserExists(user.username) == BOOL_TRUE)
    {
        changeNickname(user.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    ERROR_USERNAME_NOT_KNOWN(user.username, cmd.sender);
}