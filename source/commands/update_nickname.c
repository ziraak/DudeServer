#include "update_nickname.h"

int handleUpdateNicknameCommand(char *username, char *nickname)
{
    if (checkUser(username) == BOOL_TRUE)
    {
        changeNickname(username, nickname);
        return RPL_SUCCESS;
    }
    return ERR_USERNAME_NOT_KNOWN;
}