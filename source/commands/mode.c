//
// Created by osboxes on 27/05/15.
//

#include "mode.h"

int hasFlag(char flag, char* flags)
{
    int i;
    for(i = 0; flags[i] != '\0'; i++)
    {
        if(flags[i] == flag)
        {
            return BOOL_TRUE;
        }
    }

    return BOOL_FALSE;
}

void handleSFlag(char *channelName, char *flags)
{
    if(hasFlag('s', flags) == BOOL_TRUE)
    {
        setChannelVisibility(channelName, (flags[0] == '+') ? BOOL_TRUE : BOOL_FALSE);
    }
}

int handleModeCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0], *flags = cmd.parameters[1];

    if(strlen(flags) < 2 || (flags[0] != '-' && flags[0] != '+'))
    {
        return ERR_NEEDMOREPARAMS;
    }

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if(userHasChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    char* role = getUserRole(channelName, currentUser.username);
    if(strcmp(role, USER_ROLE_OPERATOR) != 0)
    {
        return ERR_CHANOPPRIVSNEEDED;
    }

    handleSFlag(channelName, flags);

    return RPL_SUCCESS;
}