#include "names.h"


int handleNamesCommand(commandStruct cmd)
{
    char *channelName = cmd.parameters[0];
    channelUser *channelUserStruct;

    if (cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }
    if (checkChannel(channelName) == NULL)
    {
        return ERR_NOSUCHCHANNEL;
    }

    channelUserStruct = getUsersFromChannel(channelName);

    char *users = "";
    int i = 0;
    while (channelUserStruct[i].username != NULL)
    {
        strcat(users, " ");
        strcat(users, channelUserStruct[i].username);
        i++;
    }
    sslSend(users);
    channelUser_free(channelUserStruct);
    return RPL_SUCCESS;
}