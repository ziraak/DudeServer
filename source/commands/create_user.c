#include "create_user.h"

int handleCreateUserCommand(char *message)
{
    char *username = NULL, *password = NULL;
    int offset = substringCharacter(message, &username);
    substringCharacter(message += offset, &password);

    if (checkUser(username) == EXIT_SUCCESS)
    {
        return ERR_USERNAMEINUSE;
    }
    else
    {
        if (strlen(password) > 5)
        {
            createNewUser(username, password);
            return RPL_SUCCESS;
        }
        else
        {
            return ERR_PASSWORDTOOSHORT;
        }
    }
}