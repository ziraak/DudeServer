#include "names.h"

void handleNames(int client)
{
    int result;
    channelInfo *channels = getUserChannels(getClient(client)->user.username, &result);

    if(result < 0)
    {
        return;
    }

    int i;
    for(i = 0; i < result; i++)
    {
        handleChannelNames(channels[i].name, client);
    }

    channelInfos_free(channels, result);
}

void handleChannelNames(char* channelName, int client)
{
    if (checkChannel(channelName) == BOOL_FALSE)
    {
        _errorNoSuchChannel(channelName, client);
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

    char *users = MALLOC(mallocUsersLength + mallocRolesLength + 1 + strlen(channelName));
    char *roles = MALLOC(mallocRolesLength);

    sprintf(users, "%i %s", RPL_NAMREPLY, channelName);
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
    sendToClient(client, users);
    FREE(users);
    FREE(roles);
    userInfos_free(userInfoStruct, result);
}