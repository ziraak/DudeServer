#include "names.h"


int handleNamesCommand(commandStruct cmd)
{
    timeStart;
    char *channelName = cmd.parameters[0];

    if (cmd.parameterCount < 1)
    {
        timeEnd("handleNamesCommand");
        return ERR_NEEDMOREPARAMS;
    }
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        timeEnd("handleNamesCommand");
        return ERR_NOSUCHCHANNEL;
    }

    int result;
    userInfo *userInfoStruct;
    userInfoStruct = getChannelUsers(channelName, &result);

    size_t mallocUsersLength = 0;
    size_t mallocRolesLength = 0;
    int i = 0;
    for (i = 0; i < result; i++)
    {
        mallocUsersLength += strlen(userInfoStruct[i].username) + 4 + 1 + 1; // 4 = "300 " AND 1 = "," AND 1 = " "
        mallocRolesLength += strlen(userInfoStruct[i].role) + 1; // 1 = ","
    }

    char *users = MALLOC(mallocUsersLength + mallocRolesLength);
    char *roles = MALLOC(mallocRolesLength);
    bzero(users, mallocUsersLength + mallocRolesLength);
    bzero(roles, mallocRolesLength);

    sprintf(users, "%i", RPL_NAMREPLY);
    strcat(users, " ");
    strcat(roles, "");
    for (i = 0; i < result; i++)
    {
        strcat(users, userInfoStruct[i].username);
        strcat(roles, userInfoStruct[i].role);

        if (i < result - 1)
        {
            strcat(users, ",");
            strcat(roles, ",");
        }
    }
    strcat(users, " ");
    strcat(users, roles);
    sslSend(users);
    FREE(users);
    FREE(roles);
    userInfos_free(userInfoStruct, result);
    timeEnd("handleNamesCommand");
    return RPL_NOREPLY;
}