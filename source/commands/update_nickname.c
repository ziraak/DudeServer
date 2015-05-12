#include "update_nickname.h"

int handleUpdateNicknameCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    if (checkUser(currentUser.username) == BOOL_TRUE)
    {
        changeNickname(currentUser.username, cmd.parameters[0]);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}