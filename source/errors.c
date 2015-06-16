//
// Created by osboxes on 14/06/15.
//

#include "errors.h"

void _errorNotOnChannel(char *channel, char *username, int client)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s", ERR_NOTONCHANNEL, channel, username);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorNoSuchChannel(char* channel, int client)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s", ERR_NOSUCHCHANNEL, channel);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorNeedMoreParameter(char* msg, int count, int client)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %i '%s'", ERR_CHANOPPRIVSNEEDED, count, msg);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorChannelPrivilegesNeeded(char* channel, int client)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s", ERR_CHANOPPRIVSNEEDED, channel);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorPasswordTooShort(int client)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %i", ERR_PASSWORDTOOSHORT, MINIMUM_PASSWORD_LENGTH);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorUsernameNotKnown(char* username, int client)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s", ERR_USERNAME_NOT_KNOWN, username);
    sendToClient(client, buffer);
    FREE(buffer);
}

void _errorUnknownModeFlag(char flag, int client)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %c", ERR_UMODEUNKNOWNFLAG, flag);
    sendToClient(client, buffer);
    FREE(buffer);
}
