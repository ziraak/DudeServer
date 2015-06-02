#include "names.h"


int handleNamesCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    channelUser *channelUserStruct;

    if (cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    channelUserStruct = NULL;

    size_t mallocUsersLength = 0;
    size_t mallocRolesLength = 0;
    int i = 0;
    while (channelUserStruct[i].username != NULL)
    {
        mallocUsersLength += strlen(channelUserStruct[i].username) + 6 + 1 + 1; // 6 = "REPLY " AND 1 = "," AND 1 = " "
        mallocRolesLength += strlen(channelUserStruct[i].role) + 1; // 1 = ","
        i++;
    }

    char *users = malloc(mallocUsersLength + mallocRolesLength);
    char *roles = malloc(mallocRolesLength);
    strcat(users, "REPLY ");
    strcat(roles, "");
    i = 0;
    while (channelUserStruct[i].username != NULL)
    {
        strcat(users, ",");
        strcat(users, channelUserStruct[i].username);
        strcat(roles, ",");
        strcat(roles, channelUserStruct[i].role);
        i++;
    }
    strcat(users, " ");
    strcat(users, roles);
    sslSend(users);
    free(users);
    free(roles);
    channelUser_free(channelUserStruct);
    return RPL_SUCCESS;
}